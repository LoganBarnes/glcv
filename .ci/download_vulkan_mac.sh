#!/usr/bin/env bash
pushd $1
VULKAN_VERSION=1.1.82.0
VULKAN_TAR_FILE="vulkansdk-macos-${VULKAN_VERSION}.tar.gz"
VULKAN_URL="https://sdk.lunarg.com/sdk/download/${VULKAN_VERSION}/mac/${VULKAN_TAR_FILE}?Human=true;u="
mkdir vulkan-mac \
&& travis_retry wget --no-check-certificate -O - ${VULKAN_URL} \
| tar --strip-components=1 -xz -C vulkan-mac
export VULKAN_SDK=$1/vulkan-mac/macOS
popd
