#!/bin/bash

toolkit_name=${1:-toolkit_name}
current_date=$(date +"%Y.%m%d")
short_hash=$(git rev-parse --short HEAD)
dir="ocp/toolkit/$current_date.$short_hash"
xzfile="${toolkit_name}-$current_date.$short_hash.tar.xz"

check_git() {
    # Check if there are uncommitted changes in the current repository
    if ! git diff-index --quiet HEAD --; then
        echo "There are uncommitted changes in the current repository. Skipping..."
        return 1
    fi

    # Check each submodule for uncommitted changes
    if ! git submodule status --recursive | awk '$1 ~ /^+/ {exit 1}'; then
        echo "There are uncommitted changes in a submodule. Skipping..."
        return 1
    fi
    return 0
}

publish() {
    rm -rf ocp
    rm ${toolkit_name}-*.tar.xz

    mkdir -p "$dir"
    tar -cJf $xzfile ocp/
    echo "Create compressed files"
    rclone copy -P $xzfile ali:ocp-toolkit
    rm -rf ocp
    rm ${toolkit_name}-*.tar.xz
}

if check_git; then
    script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
    cd $script_dir/..
    echo "No uncommitted changes detected. Proceeding with script..."
    publish
    cd -
else
    echo "Skipping script due to uncommitted changes."
fi
