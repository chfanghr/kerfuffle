#!/usr/bin/env bash

set -e

BUILD_FOLDER="cmake-debug-build"
CC="$(command -v clang-9)"
CXX="$(command -v clang++-9)"

mkdir ${BUILD_FOLDER}
conan install . -s compiler=clang -s compiler.version=9 -s compiler.libcxx=libstdc++11 -s build_type=Debug --install-folder=${BUILD_FOLDER} --build

cd ${BUILD_FOLDER}
cmake -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -DENABLE_COVERAGE=OFF ..
make -j8

make test
