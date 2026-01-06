#!/bin/bash

BUILD_SHARED=OFF
BUILD_EXAMPLE=OFF

while [[ $# -gt 0 ]]; do
    case "$1" in
        --build_example)
            BUILD_EXAMPLE=ON
            ;;
        --build_shared)
            BUILD_SHARED=ON
            ;;
        *)
            echo "Error: Invalid parameter!"
            echo "Usage: $0 [--build_example] [--build_shared]"
            exit 1
            ;;
    esac
    shift
done

cmake -B build \
    -DGLOBAL_HOTKEY_BUILD_SHARED="$BUILD_SHARED" \
    -DGLOBAL_HOTKEY_BUILD_EXAMPLE="$BUILD_EXAMPLE" \
    -DGLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK=ON && \
cmake --build build -j && \
cmake --install build --prefix install

unset BUILD_SHARED
unset BUILD_EXAMPLE
