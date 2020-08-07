MKDIR cmake-msvc-x86_64-windows-build-release
conan install . --install-folder=cmake-msvc-x86_64-windows-build-release -s build_type=MinSizeRel
CD cmake-msvc-x86_64-windows-build-release
cmake -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -A x64 -v ..
msbuild .\ALL_BUILD.vcxproj
ctest -C "Release"
