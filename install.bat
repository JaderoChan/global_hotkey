cmake -B build -DGLOBAL_HOTKEY_BUILD_EXAMPLE=OFF
make -C build -j
cmake --install build --prefix=install
