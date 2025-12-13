cmake -B build -DGLOBAL_HOTKEY_BUILD_EXAMPLE=OFF
cmake --build build -j
cmake --install build --prefix install
