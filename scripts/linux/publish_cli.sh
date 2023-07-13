#!/bin/bash

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
  rm cli*.tar.xz
  current_date=$(date +"%Y.%m%d")
  short_hash=$(git rev-parse --short HEAD)
  dir="ocp/cli/$current_date.$short_hash"
  mkdir -p "$dir"
  go build -o $dir/ocp

  echo "Build the cli tool $dir/ocp"

  # today=$(date +"%Y%m%d")
  version=$current_date.$short_hash
  distro_version=$(. $script_dir/distro_version.sh)
  tar -cJf cli-$version-$distro_version.tar.xz ocp/
  tar -cJf cli-latest-$distro_version.tar.xz ocp/
  echo "Create compressed files"
  rclone copy -P cli-$version-$distro_version.tar.xz ali:ocp-download/cli
  rclone copy -P cli-latest-$distro_version.tar.xz ali:ocp-download/cli
  echo "Upload compressed files"
  rm -rf ocp
  rm cli*.tar.xz
}

if check_git; then
  script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
  cd $script_dir/../../cli
  echo "No uncommitted changes detected. Proceeding with script..."
  publish
  cd -
else
  echo "Skipping script due to uncommitted changes."
fi
