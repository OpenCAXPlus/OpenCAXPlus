#!/bin/sh

# today=$(date +"%Y%m%d")
. /etc/os-release
distro=$ID
version=$VERSION_ID
distro_version="$(echo "$distro" | tr '[:upper:]' '[:lower:]')-$version"

software="ocp-cli"
version=${1:-"latest"}
system=${2:-"$distro_version"}

# prepare some variables
url="http://ocp-download.oss-cn-hongkong.aliyuncs.com/$software-$version-$system.tar.xz"
filename=$(basename "$url")
dir_name=${filename%.tar.xz}
download_dir="/tmp"
merge_dir="$HOME/ocp"

echo "Installing $software"
echo "version=$version"
echo "system=$system"
echo

# download the compressed file to /tmp folder
if [ -f "$download_dir/$filename" ]; then
    echo "You have downloaded $filename before."
    echo "It will be reused, and we won't download it again."
    echo
else

    # Try to ping Cloudflare's public DNS server.
    if
        ping -c 1 47.75.18.211 &
        >/dev/null
    then
        wget -P "$download_dir" "$url"
    else
        echo "The computer cannot connect to Aliyun HongKong OSS server."
        echo "You need to manually download the file and put it into /tmp folder."
        echo "If needed, please contact support@opencax.plus for help."
        echo
    fi
fi

if [ ! -f "$download_dir/$filename" ]; then
    echo "Could find the $filename in $download_dir."
    echo "Stop without any installation."
    echo
else
    # extract the compressed file to /tmp folder
    echo "Extract content of $download_dir/$filename"
    echo
    tar -xJf "$download_dir/$filename" -C "$download_dir"

    # install the contents of the extracted folder to the $HOME/ocp folder
    mkdir -p $HOME/ocp
    echo "Install contents to $merge_dir"
    echo
    rsync -av --ignore-existing "$download_dir/ocp/" "$merge_dir"

    for dir in $download_dir/ocp/*; do # the cli and sdk level
        if [ ! -d "$dir" ]; then
            continue
        fi
        dir_base=$(basename "$dir")
        count=0
        dist_name=""
        for i in $dir/*; do
            if [ -d "$i" ]; then
                # increment count
                count=$((count + 1))
                # if it's the first directory, save the name
                if [ $count -eq 1 ]; then
                    dist_name=$(basename "$i")
                fi
            fi
        done

        if [ $count -gt 1 ]; then
            # Print a warning message
            echo "Warning: More than one subdirectory found in $subdir"
            echo "Warning: This probably is not the intended behavior"
        fi

        echo "Establishing link $dir_base/latest => $dir_base/$dist_name"
        ln -sf $merge_dir/$dir_base/$dist_name $merge_dir/$dir_base/latest
    done

    # remove the extracted folder in /tmp
    rm -rf "$download_dir/ocp/"

    echo "export PATH=\$PATH:$HOME/ocp/cli/latest" >> ~/.bashrc
fi
