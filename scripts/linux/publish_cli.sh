#!/bin/bash

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
cd $script_dir/../../cli

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
  rm ocp-*.tar.xz
  current_date=$(date +"%Y.%m%d")
  short_hash=$(git rev-parse --short HEAD)
  dir="ocp/cli/$current_date.$short_hash"
  mkdir -p "$dir"
  go build -o $dir/ocp

  echo "Build the cli tool $dir/ocp"

  today=$(date +"%Y%m%d")
  distro_version=$(. $script_dir/distro_version.sh)
  tar -cJf ocp-cli-$today-$distro_version.tar.xz ocp/
  tar -cJf ocp-cli-latest-$distro_version.tar.xz ocp/
  echo "Create compressed files"
  rclone copy -P ocp-cli-$today-$distro_version.tar.xz ali:ocp-download
  rclone copy -P ocp-cli-latest-$distro_version.tar.xz ali:ocp-download
  echo "Upload compressed files"
  rm -rf ocp
  rm ocp-*.tar.xz
}

if check_git; then
  echo "No uncommitted changes detected. Proceeding with script..."
  publish
else
  echo "Skipping script due to uncommitted changes."
fi

cd -
