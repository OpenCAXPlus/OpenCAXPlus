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
  script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
  cd $script_dir/../..

  rm *.tar.xz

  rm -rf out
  cmake --preset="linux-gnu-Release" -S "."
  cmake --build --preset="linux-gnu-Release"
  cmake --build --preset="linux-gnu-Release" --target install

  current_date=$(date +"%Y.%m%d")
  short_hash=$(git rev-parse --short HEAD)
  cli_dir="ocp/cli/$current_date.$short_hash"
  sdk_dir="ocp/sdk/$current_date.$short_hash"
  mkdir -p "$cli_dir"
  mkdir -p "$sdk_dir"

  rsync -av --ignore-existing out/install/linux-gnu-Release/* $sdk_dir

  script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
  today=$(date +"%Y%m%d")
  distro_version=$(. $script_dir/distro_version.sh)
  tar -cJf ocp-cli-$today-$distro_version.tar.xz ocp/
  tar -cJf ocp-cli-latest-$distro_version.tar.xz ocp/

  rclone copy -P ocp-cli-$today-$distro_version.tar.xz r2:mupro-download
  rclone copy -P ocp-cli-latest-$distro_version.tar.xz r2:mupro-download
  rm -rf ocp
  rm ocp-*.tar.xz
}

if check_git; then
  echo "No uncommitted changes detected. Proceeding with script..."
  publish
else
  echo "Skipping script due to uncommitted changes."
fi
