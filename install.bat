cmake -B build -DGLOBAL_HOTKEY_BUILD_EXAMPLE=OFF -DGLOBAL_HOTKEY_BUILD_SHARED=ON
make -C build -j
cmake --install build --prefix=install
