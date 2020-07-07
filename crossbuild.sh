#!/usr/bin/env bash

PLATFORMS=(
  "arm-linux-gnueabi"
  "arm-linux-gnueabihf"
  "powerpc64le-linux-gnu"
  "aarch64-linux-gnu"
  "mipsel-linux-gnu"
  "i386-apple-darwin"
  "x86_64-apple-darwin"
  "i686-w64-mingw32"
  "x86_64-w64-mingw32"
)

for p in ${PLATFORMS[@]}; do
  echo -e "Building for: \033[94m${p}\033[0m"
  docker run --rm -e CROSS_TRIPLE=${p} -v $(PWD):/workdir multiarch/crossbuild /bin/sh -c "./docker-entrypoint.sh"
done
