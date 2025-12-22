BUILD_EXAMPLE="OFF"

if [ $# -gt 0 ]; then
    PARAM=$(echo "$1" | tr '[:lower:]' '[:upper:]')
    if [ "$PARAM" != "ON" ] && [ "$PARAM" != "OFF" ]; then
        echo "Error: Invalid parameter!"
        echo "Usage: $0 [ON/OFF [--build_example_use_hook]]"
        exit 1
    fi
    BUILD_EXAMPLE="$PARAM"
    BUILD_EXAMPLE_USE_HOOK="ON"
fi

cmake -B build -DGLOBAL_HOTKEY_DISABLE_REGISTER=ON -DGLOBAL_HOTKEY_BUILD_EXAMPLE="$BUILD_EXAMPLE" -DGLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK="$BUILD_EXAMPLE_USE_HOOK"
cmake --build build -j
cmake --install build --prefix install
