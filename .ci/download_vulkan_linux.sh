#!/usr/bin/env bash
pushd $1
VULKAN_VERSION=1.1.77.0
VULKAN_TAR_FILE="vulkansdk-linux-x86_64-$VULKAN_VERSION.tar.gz"
VULKAN_URL="https://vulkan.lunarg.com/sdk/download/$VULKAN_VERSION/linux/$VULKAN_TAR_FILE"
mkdir vulkan \
&& travis_retry wget --no-check-certificate --quiet -O - ${VULKAN_URL} \
| tar --strip-components=1 -xz -C vulkan
export VULKAN_SDK=$1/vulkan/x86_64
popd
