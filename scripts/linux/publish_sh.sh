#!/bin/bash

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
cd $script_dir/../..
rclone copy -P scripts/linux/install.sh ali:ocp-download
rclone copy -P packages.yml ali:ocp-download
cd -