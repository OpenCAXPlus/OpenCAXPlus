#!/bin/bash

check_git() {
  # Check if there are uncommitted changes in the current repository
  if ! git diff-index --quiet HEAD --; then
    echo "There are uncommitted changes in the current repository. Skipping..."
    # return 1
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

  rm -rf out
  cmake --preset="linux-gnu-Release" -S "."
  cmake --build --preset="linux-gnu-Release"
  cmake --build --preset="linux-gnu-Release" --target install

  current_date=$(date +"%Y.%m%d")
  short_hash=$(git rev-parse --short HEAD)
  dir="ocp/sdk/$current_date.$short_hash"
  mkdir -p "$dir"

  cp -r out/install/linux-gnu-Release/* $dir

  # today=$(date +"%Y%m%d")
  version=$current_date.$short_hash
  distro_version=$(. $script_dir/distro_version.sh)
  tar -cJf ocp-sdk-$version-$distro_version.tar.xz ocp/
  tar -cJf ocp-sdk-latest-$distro_version.tar.xz ocp/

  rclone copy -P ocp-sdk-$version-$distro_version.tar.xz ali:ocp-download
  rclone copy -P ocp-sdk-latest-$distro_version.tar.xz ali:ocp-download
  rm -rf ocp
  rm ocp-*.tar.xz
}

if check_git; then
  script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
  cd $script_dir/../..
  echo "No uncommitted changes detected. Proceeding with script..."
  publish
  cd -
else
  echo "Skipping script due to uncommitted changes."
fi
