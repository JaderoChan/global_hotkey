@echo off

set BUILD_EXAMPLE=OFF
set BUILD_EXAMPLE_USE_HOOK=OFF

if not "%1"=="" (
    if /i not "%1"=="ON" if /i not "%1"=="OFF" (
        echo Error: Invalid parameter!
        echo Usage: %0 [ON/OFF [--build_example_use_hook]]
        exit /b 1
    )
    set BUILD_EXAMPLE=%1

    if not "%2"=="" (
        if /i not "%2"=="--build_example_use_hook" (
            echo Error: Invalid parameter!
            echo Usage: %0 [ON/OFF [--build_example_use_hook]]
            exit /b 1
        )
        set BUILD_EXAMPLE_USE_HOOK=ON
    )
)

cmake -B build -DGLOBAL_HOTKEY_BUILD_EXAMPLE=%BUILD_EXAMPLE% -DGLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK=%BUILD_EXAMPLE_USE_HOOK%
cmake --build build -j
cmake --install build --prefix install

set "BUILD_EXAMPLE="
set "BUILD_EXAMPLE_USE_HOOK="
