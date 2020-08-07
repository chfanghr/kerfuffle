#!/usr/bin/env bash

wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key 2>/dev/null | sudo apt-key add -
sudo add-apt-repository 'deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-9 main' -y
sudo apt-get update -q
sudo apt-get install -y clang-9 lld-9 libc++-9-dev libc++abi-9-dev clang-tools-9