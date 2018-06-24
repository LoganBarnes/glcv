#!/usr/bin/env bash
pushd $1
VULKAN_VERSION=1.1.77.0
VULKAN_TAR_FILE="vulkansdk-linux-x86_64-$VULKAN_VERSION.tar.gz"
VULKAN_URL="https://vulkan.lunarg.com/sdk/download/$VULKAN_VERSION/linux/$VULKAN_TAR_FILE"
mkdir vulkan-linux \
&& travis_retry wget --no-check-certificate -O - ${VULKAN_URL} \
| tar --strip-components=1 -xz -C vulkan-linux
export VULKAN_SDK=$1/vulkan-linux/x86_64
popd
