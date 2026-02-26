#include <cstdio>               // printf
#include <atomic>               // atomic
#include <condition_variable>   // condition_variable
#include <string>

#include <global_hotkey/global_hotkey.hpp>

#ifdef GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK
    #ifndef GLOBAL_HOTKEY_WIN
        #include <unistd.h> // getuid

        static bool isRootPermission()
        {
            return getuid() == 0;
        }
    #endif // !_GLOBAL_HOTKEY_WUIN

    #ifdef GLOBAL_HOTKEY_MAC
        #include <ApplicationServices/ApplicationServices.h>

        static bool isAssistiveAccessible()
        {
            return AXIsProcessTrustedWithOptions(NULL);
        }
    #endif // GLOBAL_HOTKEY_MAC
#endif // GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK

static bool isPermissionAccessible()
{
#ifdef GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK
    #ifdef GLOBAL_HOTKEY_WIN
        return true;
    #elif defined(GLOBAL_HOTKEY_MAC)
        return isRootPermission() || isAssistiveAccessible();
    #elif defined(GLOBAL_HOTKEY_LINUX)
        return isRootPermission();
    #else
        return false;
    #endif // GLOBAL_HOTKEY_WIN
#endif // GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK
    return true;
}

int main()
{
#ifdef GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK
    gbhk::GlobalHotkeyManager& ghm = gbhk::HookGlobalHotkeyManager::getInstance();
    if (!isPermissionAccessible())
    {
        printf("Permission Denied!\n");
        return 1;
    }
    const char* ghmPrefix = "Hook";
#else
    gbhk::GlobalHotkeyManager& ghm = gbhk::RegisterGlobalHotkeyManager::getInstance();
    const char* ghmPrefix = "Register";
#endif // GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK

    gbhk::KeyCombination hotkey1(gbhk::CTRL, 'J');
    gbhk::KeyCombination hotkey2(gbhk::CTRL | gbhk::SHIFT, 'J');
    gbhk::KeyCombination hotkey3(gbhk::CTRL | gbhk::SHIFT, gbhk::Key_Backspace);

    int rc = ghm.run();
    if (rc != gbhk::RC_SUCCESS)
    {
        printf("Failed to run the Global Hotkey Manager service. Error message: %s.\n",
            gbhk::getReturnCodeMessage(rc).c_str());
        return 1;
    }
    printf("Run the %s GHM service successfully!\n", ghmPrefix);

    rc = ghm.registerHotkey(hotkey1, [=]() { printf("%s be triggered!\n", hotkey1.toString().c_str()); });
    if (rc != gbhk::RC_SUCCESS)
    {
        ghm.stop();
        printf("Failed to register the hotkey [%s]. Error message: %s.\n",
            hotkey1.toString().c_str(),
            gbhk::getReturnCodeMessage(rc).c_str());
        return 1;
    }
    printf("Register the hotkey: [%s] successfully\n", hotkey1.toString().c_str());

    // The hotkey 2 is auto repeat.
    rc = ghm.registerHotkey(hotkey2, [=]() { printf("%s be triggered!\n", hotkey2.toString().c_str()); }, true);
    if (rc != gbhk::RC_SUCCESS)
    {
        ghm.stop();
        printf("Failed to register the hotkey [%s]. Error message: %s.\n",
            hotkey2.toString().c_str(),
            gbhk::getReturnCodeMessage(rc).c_str());
        return 1;
    }
    printf("Register the hotkey: [%s] (auto repeat) successfully\n", hotkey2.toString().c_str());

    std::atomic<bool> shouldClose(false);
    std::condition_variable cv;
    rc = ghm.registerHotkey(hotkey3, [&]()
    {
        printf("%s be triggered!\n", hotkey3.toString().c_str());
        shouldClose = true;
        cv.notify_one();
    });
    if (rc != gbhk::RC_SUCCESS)
    {
        ghm.stop();
        printf("Failed to register the hotkey [%s]. Error message: %s.\n",
            hotkey3.toString().c_str(),
            gbhk::getReturnCodeMessage(rc).c_str());
        return 1;
    }
    printf("Register the hotkey: [%s] successfully\n", hotkey3.toString().c_str());
    printf("Press the [%s] to exit!\n\n", hotkey3.toString().c_str());

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> dummyLocker(dummyMtx);
    cv.wait(dummyLocker, [&]() { return shouldClose.load(); });

    printf("Exit...\n");
    rc = ghm.stop();
    if (rc != gbhk::RC_SUCCESS)
    {
        printf("Failed to stop the Global Hotkey Manager service. Error message: %s.\n",
            gbhk::getReturnCodeMessage(rc).c_str());
        return 1;
    }
    printf("The Global Hotkey Manager service is stoped!\n");

    return 0;
}
