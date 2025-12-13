cmake -B build -DGLOBAL_HOTKEY_BUILD_EXAMPLE=OFF -DGLOBAL_HOTKEY_DISABLE_REGISTER=ON -DGLOBAL_HOTKEY_DISABLE_HOOK=OFF
cmake --build build -j
cmake --install build --prefix install
