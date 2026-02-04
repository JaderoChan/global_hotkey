@echo off

set SCRIPT_FILENAME=%0
set BUILD_SHARED=OFF
set BUILD_EXAMPLE=OFF
set BUILD_EXAMPLE_USE_HOOK=OFF
set INSTALL_PREFIX=

goto :parameterHandler

:echoHelperAndExit
    echo Error: Invalid parameter!
    echo Usage: %SCRIPT_FILENAME% [--build-example] [--build-example-use-hook] [--build-shared] [--prefix ^<install prefix^>]
    exit /b 1

:parameterHandler
    if "%~1" == "" goto :done

    if /i "%~1" == "--build-example" (
        set BUILD_EXAMPLE=ON
    ) else if /i "%~1" == "--build-example-use-hook" (
        set BUILD_EXAMPLE_USE_HOOK=ON
    ) else if /i "%~1" == "--build-shared" (
        set BUILD_SHARED=ON
    ) else if /i "%~1" == "--prefix" (
        if "%~2" == "" (
            goto :echoHelperAndExit
        ) else (
            set INSTALL_PREFIX=%~2
            shift
        )
    ) else (
        goto :echoHelperAndExit
    )

    shift
    goto :parameterHandler

:done

if not "%INSTALL_PREFIX%" == "" (
    set INSTALL_PREFIX_CMD=--prefix "%INSTALL_PREFIX%"
)

cmake -B build ^
    -DGLOBAL_HOTKEY_BUILD_SHARED=%BUILD_SHARED% ^
    -DGLOBAL_HOTKEY_BUILD_EXAMPLE=%BUILD_EXAMPLE% ^
    -DGLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK=%BUILD_EXAMPLE_USE_HOOK% && ^
cmake --build build --config release -j && ^
cmake --install build %INSTALL_PREFIX_CMD%

set "SCRIPT_FILENAME="
set "BUILD_SHARED="
set "BUILD_EXAMPLE="
set "BUILD_EXAMPLE_USE_HOOK="
set "INSTALL_PREFIX="
set "INSTALL_PREFIX_CMD="

exit /b 0
