## Create and upload the deb

Here I'm using the rclone tool to upload the files, you can use other tools.
```sh
ocp build -b Release -t package
cd repos/apt
reprepro includedeb all ../../out/build/linux-intel-Release/ocp_0.0.2_amd64.deb
cd ..
rclone copyto --progress apt ali:ocp-apt-repos
```


```
# .config/rclone/rclone.conf
[ali]
type = s3
provider = Alibaba
access_key_id = LTAI5tCCgDxab3RPEUbyxfad
secret_access_key = **
endpoint = oss-cn-hongkong.aliyuncs.com
acl = public-read
storage_class = STANDARD

```


## Use the private repo
We don't have a certificate, so have to use http for now

```sh
sudo wget -O - http://apt.repos.opencax.plus/ocp.gpg.key | sudo apt-key add -
sudo wget http://apt.repos.opencax.plus/ocp.list -O /etc/apt/sources.list.d/ocp.list
```