#!/usr/bin/env bash
pushd $1
CMAKE_VERSION=3.12
CMAKE_TAR_FILE="cmake-$CMAKE_VERSION.0-Linux-x86_64.tar.gz"
CMAKE_URL="https://cmake.org/files/v$CMAKE_VERSION/$CMAKE_TAR_FILE"
mkdir cmake \
&& travis_retry wget --no-check-certificate --quiet -O - ${CMAKE_URL} \
| tar --strip-components=1 -xz -C cmake
export PATH=$1/cmake/bin:${PATH}
popd
