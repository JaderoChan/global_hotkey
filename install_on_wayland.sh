#!/bin/bash

SCRIPT_FILENAME=$0
BUILD_SHARED=OFF
BUILD_EXAMPLE=OFF
INSTALL_PREFIX=

echo_helper_and_exit() {
    echo "Error: Invalid parameter!"
    echo "Usage: $SCRIPT_FILENAME [--build-example] [--build-shared] [--prefix <install prefix>]"
    exit 1
}

while [[ $# -gt 0 ]]; do
    case "${1,,}" in
        --build-example)
            BUILD_EXAMPLE=ON
            ;;
        --build-shared)
            BUILD_SHARED=ON
            ;;
        --prefix)
            if [[ -z "$2" ]]; then
                echo_helper_and_exit
            fi
            INSTALL_PREFIX=$2
            shift
            ;;
        *)
            echo_helper_and_exit
            ;;
    esac
    shift
done

INSTALL_PREFIX_CMD=
if [[ "$INSTALL_PREFIX" ]]; then
    INSTALL_PREFIX_CMD="--prefix $INSTALL_PREFIX"
fi

cmake -B build \
    -DGLOBAL_HOTKEY_BUILD_SHARED=$BUILD_SHARED \
    -DGLOBAL_HOTKEY_BUILD_EXAMPLE=$BUILD_EXAMPLE \
    -DGLOBAL_HOTKEY_DISABLE_REGISTER=ON \
    -DGLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK=ON && \
cmake --build build --config release -j && \
cmake --install build $INSTALL_PREFIX_CMD

unset SCRIPT_FILENAME
unset BUILD_SHARED
unset BUILD_EXAMPLE
unset INSTALL_PREFIX
unset INSTALL_PREFIX_CMD

exit 0
