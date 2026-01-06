@echo off

set BUILD_SHARED=OFF
set BUILD_EXAMPLE=OFF
set BUILD_EXAMPLE_USE_HOOK=OFF

:parameterHandler
    if "%~1" == "" goto :done

    if /i "%~1" == "--build_example" (
        set BUILD_EXAMPLE=ON
    ) else if /i "%~1" == "--build_example_use_hook" (
        set BUILD_EXAMPLE_USE_HOOK=ON
    ) else if /i "%~1" == "--build_shared" (
        set BUILD_SHARED=ON
    ) else (
        echo Error: Invalid parameter!
        echo Usage: %0 [--build_example] [--build_example_use_hook] [--build_shared]
        exit /b 1
    )

    shift
    goto :parameterHandler

call :parameterHandler %*
:done

cmake -B build ^
    -DGLOBAL_HOTKEY_BUILD_SHARED=%BUILD_SHARED% ^
    -DGLOBAL_HOTKEY_BUILD_EXAMPLE=%BUILD_EXAMPLE% ^
    -DGLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK=%BUILD_EXAMPLE_USE_HOOK% && ^
cmake --build build -j && ^
cmake --install build --prefix install

set "BUILD_SHARED="
set "BUILD_EXAMPLE="
set "BUILD_EXAMPLE_USE_HOOK="
