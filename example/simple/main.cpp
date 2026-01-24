#include <cstdio>               // printf
#include <atomic>               // atomic
#include <condition_variable>   // condition_variable
#include <mutex>                // mutex
#include <stdexcept>            // runtime_error
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

#define THROW_RT_ERR(errmsg, code) (throw std::runtime_error((errmsg) + gbhk::getReturnCodeMsg(code)))

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

static void hotkeyTriggered1()
{
    printf("Hotkey 1 be triggered!\n");
}

static void hotkeyTriggered2()
{
    printf("Hotkey 2 be triggered!\n");
}

int main()
{
#ifdef GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK
    gbhk::GlobalHotkeyManager& ghm = gbhk::HookGlobalHotkeyManager::getInstance();
    if (!isPermissionAccessible())
        throw std::runtime_error("Permission Denied!");
    const char* ghmPrefix = "Hook";
#else
    gbhk::GlobalHotkeyManager& ghm = gbhk::RegisterGlobalHotkeyManager::getInstance();
    const char* ghmPrefix = "Register";
#endif // GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK

    gbhk::KeyCombination hotkey1(gbhk::CTRL, 'J');
    gbhk::KeyCombination hotkey2(gbhk::CTRL | gbhk::SHIFT, 'J');
    gbhk::KeyCombination hotkey3(gbhk::CTRL | gbhk::SHIFT, gbhk::Key_Backspace);

    int rc = ghm.initialize();
    if (rc != gbhk::RC_SUCCESS)
    {
        THROW_RT_ERR("Failed to initialize the Global Hotkey Manager: ", rc);
    }
    printf("Successfully initialize the %s GHM!\n", ghmPrefix);

    rc = ghm.add(hotkey1, &hotkeyTriggered1);
    if (rc != gbhk::RC_SUCCESS)
    {
        ghm.uninitialize();
        THROW_RT_ERR("Failed to add the hotkey 1: ", rc);
    }
    printf("Successfully add the hotkey: [%s]\n", hotkey1.toString().c_str());

    // The hotkey 2 is auto repeat.
    rc = ghm.add(hotkey2, &hotkeyTriggered2, true);
    if (rc != gbhk::RC_SUCCESS)
    {
        ghm.uninitialize();
        THROW_RT_ERR("Failed to add the hotkey 2: ", rc);
    }
    printf("Successfully add the hotkey: [%s] (auto repeat)\n", hotkey2.toString().c_str());

    std::atomic<bool> shouldClose(false);
    std::condition_variable cv;
    rc = ghm.add(hotkey3, [&]()
    {
        printf("Hotkey 3 be triggered!\n");
        shouldClose = true;
        cv.notify_one();
    });
    if (rc != gbhk::RC_SUCCESS)
    {
        ghm.uninitialize();
        THROW_RT_ERR("Failed to add the hotkey: ", rc);
    }
    printf("Successfully add the hotkey: [%s]\n", hotkey3.toString().c_str());
    printf("Press the [%s] to exit!\n\n", hotkey3.toString().c_str());

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cv.wait(lock, [&]() { return shouldClose.load(); });

    printf("Exit...\n");
    rc = ghm.uninitialize();
    if (rc != gbhk::RC_SUCCESS)
        THROW_RT_ERR("Failed to uninitialize the Global Hotkey Manager: ", rc);
    printf("The Global Hotkey Manager is ended!\n");

    return 0;
}
