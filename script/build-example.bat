@echo off

set "current_dir=%cd%"

cd ../
cmake -B build/example_use_register -DCMAKE_BUILD_TYPE=Release -DGLOBAL_HOTKEY_DISABLE_HOOK=ON -DGLOBAL_HOTKEY_BUILD_EXAMPLE=ON
cmake --build build/example_use_register -j
cmake -B build/example_use_hook -DCMAKE_BUILD_TYPE=Release -DGLOBAL_HOTKEY_DISABLE_REGISTER=ON -DGLOBAL_HOTKEY_BUILD_EXAMPLE=ON -DGLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK=ON
cmake --build build/example_use_hook -j

cd %current_dir%
