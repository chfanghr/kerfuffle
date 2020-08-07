MKDIR cmake-msvc-x86_64-windows-build-debug
conan install . --install-folder=cmake-msvc-x86_64-windows-build-debug build_type=Debug
CD cmake-msvc-x86_64-windows-build-debug
cmake -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -A x64 -v ..
msbuild .\ALL_BUILD.vcxproj
ctest -C "Debug"
