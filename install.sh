BUILD_EXAMPLE="OFF"
BUILD_EXAMPLE_USE_HOOK="OFF"

if [ $# -gt 0 ]; then
    PARAM=$(echo "$1" | tr '[:lower:]' '[:upper:]')

    if [ "$PARAM" != "ON" ] && [ "$PARAM" != "OFF" ]; then
        echo "Error: Invalid parameter!"
        echo "Usage: $0 [ON/OFF [--build_example_use_hook]]"
        exit 1
    fi
    BUILD_EXAMPLE="$PARAM"

    if [ $# -gt 1 ]; then
        if [ "$2" != "--build_example_use_hook" ]; then
            echo "Error: Invalid parameter!"
            echo "Usage: $0 [ON/OFF [--build_example_use_hook]]"
            exit 1
        fi
        BUILD_EXAMPLE_USE_HOOK="ON"
    fi
fi

cmake -B build -DGLOBAL_HOTKEY_BUILD_EXAMPLE="$BUILD_EXAMPLE" -DGLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK="$BUILD_EXAMPLE_USE_HOOK"
cmake --build build -j
cmake --install build --prefix install
