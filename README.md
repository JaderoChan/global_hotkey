# Global Hotkey

**English | [简体中文](doc/README_ZH.md)**

## 🚀 Features

- Independent!

    No dependencies on other libraries or frameworks!

- Works with any program!

    Even console applications!

- Cross-platform!

    Supports **Windows**, **MacOS**, and **Linux** (*Ubuntu* and *Debian* tested) with ease!

- Broad compatibility!

    **Qt**? **MFC**? No problem!

- Easy to use!

    See example code.

- `Hook-style Global Hotkey (Hook GHM)`!

    Need to implement ~~background monitoring trojan~~ some special features?

## Dependence

[Keyboard Tools](https://github.com/JaderoChan/keyboard_tools)

## 🔧 How to Build?

1. Just run the `install` script directly in the root directory to install it with one click, the available options are as follows:

    - [--build-example] (same as `GLOBAL_HOTKEY_BUILD_EXAMPLE=ON`)
    - [--build-example-use-hook] (same as `GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK=ON`) (It is not available under **Wayland** because this option is mandatory under **Wayland**)
    - [--prefix \<path\>] (Specify the installation path)

    For example, `install --build-example --prefix install` will install the library to the *./install* directory and build sample programs.

2. The project uses `CMake` for organization. Just run these few scripts to build and use it!

    ```shell
    git clone https://github.com/JaderoChan/global_hotkey.git
    cd global_hotkey
    cmake -B build
    cmake --build build
    ```

### ⚙️ Build Options

---

- `GLOBAL_HOTKEY_BUILD_SHARED` Specifies whether to build a dynamic library. Default is `OFF`. If enabled, define the `GLOBAL_HOTKEY_SHARED` macro when using the dynamic library in your project for better performance (only on **Windows**).

- `GLOBAL_HOTKEY_DISABLE_REGISTER` Specifies whether to disable `Register-style Global Hotkey (Register GHM)`. Default is `OFF`.

- `GLOBAL_HOTKEY_DISABLE_HOOK` Specifies whether to disable `Hook-style Global Hotkey (Hook GHM)`. Default is `OFF`.

- `GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY` This option is only for Hook GHM on the Windows platform. If this option is enabled, when the hotkeys 'Ctrl+Shift+ESC' and 'Ctrl+Alt+Delete' are triggered, some tricks will be used to try to prevent abnormal program behavior. Default is enable.

- `GLOBAL_HOTKEY_BUILD_EXAMPLE` Whether to build example programs. Default depends on whether the project is the main project.

- `GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK` Specifies the hotkey type used in the example program. Default is `OFF` (i.e., uses `Register GHM` in the example program).

- `GLOBAL_HOTKEY_EXAMPLE_BUILD_SIMPLE` Specifies whether to build the `simple` example program. Default is `ON`.

- `GLOBAL_HOTKEY_EXAMPLE_BUILD_EVENT_QUEUE` Specifies whether to build the `event queue` example program. Default is `ON`.

## 🚩 How to Use?

1. Obtain a `Global Hotkey Manager (GHM)` object via the `getInstance` interface.

2. Initialize the `GHM` via the `initialize` interface.

3. Add, remove, or replace hotkeys using the corresponding interfaces.

4. When a hotkey is triggered, the corresponding callback function will be executed.

5. Uninitialize the `GHM` via the `uninitialize` interface.

---

Below is example code demonstrating the basic workflow:

```cpp
GlobalHotkeyManager& ghm = RegisterGlobalHotkeyManager::getInstance();  // Get an instance of the `Register GHM`.
ghm.initialize();   // Initialize the Global Hotkey Manager.

KeyCombination hotkey1(CTRL, 'G');
KeyCombination hotkey2(CTRL, 'H');
KeyCombination hotkey3(CTRL, 'M');
ghm.add(hotkey1, &callback);                        // Bind a callback function.
ghm.add(hotkey2, [=]() { if(isOk) emitSignal(); }); // Bind a Lambda function. Emits a signal when the hotkey is triggered and the condition is true.
ghm.add(hotkey3, [=]() { printf("Hello world!") }); // Simply prints a message.

// Main loop.
while (!shouldClose)
{
    // Do Something.
}

ghm.uninitialize(); // Uninitialize the Global Hotkey Manager.
```

## 💡 Examples

[Simple Example](example/simple/main.cpp)

[Event Queue Example](example/event_queue/main.cpp)

## ❓ FAQ

### What is the License?

---

This library is licensed under the MIT License, which means you can do whatever you want with it. Although not mandatory, giving attribution in your program is highly appreciated.

### What's the difference between `Register GHM` and `Hook GHM`?

---

`Register GHM` is maintained by the operating system or desktop environment. It may conflict with global hotkeys from other applications and typically does not require administrator privileges.

`Hook GHM` is entirely based on keyboard monitoring techniques like `Hook` to implement hotkey logic. Thus, it won't conflict with other applications' global hotkeys and supports a broader range of hotkeys (e.g., on **Windows**, `Win + T` cannot be registered as a hotkey, but `Hook GHM` can achieve this). The trade-off is that it requires administrator privileges.

Unless there are special requirements, `Register GHM` should be preferred. If using `Hook GHM`, you should clearly explain to users why administrator privileges are needed and the specific purpose of the hotkey.

### Why does the library need to maintain additional threads?

---

This project decouples many modules, and these modules sometimes require their own threads.

For example, `Hook GHM` requires an additional thread for the `Keyboard Hook` module besides the `Global Hotkey Manager (GHM)`'s own worker thread. If your program needs deep customization, you can make more targeted modifications based on this.

### Does it support the Wayland window system?

---

Only `Hook GHM` is supported, and `Hook GHM` requires administrator privileges to work properly.

### Can `Register GHM` be used under the Wayland window system?

---

No. `Register GHM` on **Linux** relies on **X11**.

## 🔔 Notes

- 'Register GHM' under the MacOS system is not supported for the time being.

- Operations such as termination, addition, deletion, and hotkey replacement can only be performed after the corresponding 'GHM' has been initialized!

- Do not perform operations such as terminating, adding, deleting, and hotkey replacement in the worker thread (for the user, this is the thread where the callback function is executed when the hotkey is triggered)!

- Avoid adding an maybe invalid hotkey, as this is undefined behavior and may lead to unexpected results. This library does not conduct security checks for such operations. These operations whether be perform should be decided by the user.

- When a hotkey is triggered, its callback function will run in the worker thread of `GHM`. Therefore, the callback function of the hotkey should not perform heavy tasks to avoid blocking the worker thread. A reasonable approach is to correctly use threads, asynchronous mechanisms, or message queues (e.g. **Qt**'s signal-slot system).

- When using `Hook GHM` on **Windows**, ensure the callback function's execution time stays within the specified limit.

    *(For details, refer to [Windows LowLevelKeyboard](https://learn.microsoft.com/en-us/windows/win32/winmsg/lowlevelkeyboardproc). The **Remarks** section mentions a **Timeout** of **1000 milliseconds**.)*

- When using `Hook GHM` on the **Windows** platform, after the user presses the shortcut key `Ctrl+Shift+ESC` to call out the **Task Manager**, since the **Task Manager** will block the **LowLevelKeyboardHook** message, So it will cause `Hook GHM` to be able to receive the key press message of `Ctrl+Shift+ESC`, but there is a probability that it cannot receive the key release message (depending on the duration of your press and whether the **Task Manager** has become the focus window). Apart from `Ctrl+Shift+Esc`, there are also some other special shortcut keys (such as `Ctrl+Alt+Delete`) that can cause this problem. Particular attention should be paid when designing relevant programs. (See the compile option `GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY`, enabling this option will try to avoid this problem)

---

For some reasons, the commit records of this library before version 1.4.0 have been cleared. After the history of the commit can be [global hotkey old](https://github.com/jaderochan/global_hotkey_old) public archive to view.
