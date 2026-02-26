# Global Hotkey

**English | [简体中文](doc/README_ZH.md)**

## 🚀 Features

- Independent!

    No dependency on other libraries or frameworks!

- Works with any program!

    Even console programs!

- Cross-platform!

    **Windows**, **macOS**, **Linux** (*Ubuntu* and *Debian* tested) all supported!

- Widely compatible!

    **Qt**? **MFC**? No problem!

- Easy to use!

    See the sample code.

- Hook-based hotkeys (`Hook GHM`)!

    Multi-platform support; not limited by global uniqueness.

## Dependencies

[Keyboard Tools](https://github.com/JaderoChan/keyboard_tools)

## 🔧 How to build?

1. Run the `install` script in the repository root for one-click install. Options:

    - `--build-example` (same as `GLOBAL_HOTKEY_BUILD_EXAMPLE=ON`)
    - `--build-example-use-hook` (same as `GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK=ON`) (**Not available on Wayland**, because it is required there)
    - `--prefix <path>` (set install path)

    For example, `install --build-example --prefix install` installs the library to *./install* and builds the examples.

2. The project uses CMake. Build with just a few commands:

    ```shell
    git clone --recurse-submodules https://github.com/JaderoChan/global_hotkey.git
    cd global_hotkey
    cmake -B build
    cmake --build build
    ```

### ⚙️ Build Options

---

- `GLOBAL_HOTKEY_BUILD_SHARED` whether to build a shared library, default `OFF`. If enabled, define `GLOBAL_HOTKEY_SHARED` when using the shared library to get better performance (**Windows** only).

- `GLOBAL_HOTKEY_DISABLE_REGISTER` disable **Register GHM**, default `OFF`.

- `GLOBAL_HOTKEY_DISABLE_HOOK` disable **Hook GHM**, default `OFF`.

- `GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY` Windows-only for Hook GHM. If enabled, when `Ctrl+Shift+ESC` or `Ctrl+Alt+Delete` is triggered, some tricks are used to reduce abnormal behavior. Enabled by default.

- `GLOBAL_HOTKEY_BUILD_EXAMPLE` whether to build examples. Default depends on whether this is the top-level project.

- `GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK` whether the example uses Hook GHM. Default `OFF` (i.e., Register GHM).

- `GLOBAL_HOTKEY_EXAMPLE_BUILD_SIMPLE` build `simple` example. Default `ON`.

- `GLOBAL_HOTKEY_EXAMPLE_BUILD_EVENT_QUEUE` build `event_queue` example. Default `ON`.

## 🚩 How to use?

1. Get the `Global Hotkey Manager (GHM)` instance via `GlobalHotkeyManager::getInstance`.

2. Start the service with `GlobalHotkeyManager::run`.

3. Add, remove, or replace hotkeys using the corresponding APIs.

4. When a hotkey is triggered, its callback will be executed.

5. Stop the service with `GlobalHotkeyManager::stop`.

---

Basic usage example:

```cpp
GlobalHotkeyManager& ghm = RegisterGlobalHotkeyManager::getInstance();  // Get Register GHM instance.
ghm.run();   // Start the hotkey manager service.

KeyCombination hotkey1(CTRL, 'G');
KeyCombination hotkey2(CTRL, 'H');
KeyCombination hotkey3(CTRL, 'M');
ghm.registerHotkey(hotkey1, &callback);                        // Bind a callback function.
ghm.registerHotkey(hotkey2, [=]() { if(isOk) emitSignal(); }); // Bind a lambda; emit signal when condition is true.
ghm.registerHotkey(hotkey3, [=]() { printf("Hello world!") }); // Just print a message.

// Main loop.
while (!shouldClose)
{
    // Do Something.
}

ghm.stop(); // Release the hotkey manager.
```

## 💡 Examples

[Simple example](example/simple/main.cpp)

[Event queue example](example/event_queue/main.cpp)

## ❓ FAQ

### What license is used?

---

This library uses the MIT license, which means you can do anything with it. Attribution is not required but appreciated!

### What is the difference between `Register GHM` and `Hook GHM`?

---

`Register GHM` is maintained by the OS or desktop environment. Typically only one global hotkey with the same combination can exist system-wide, and it generally does not require admin privileges.

`Hook GHM` is fully based on keyboard listening technologies such as hooks and implements hotkey logic on top. It does not conflict with other apps’ global hotkeys and allows broader combinations (e.g., on **Windows**, `Win + T` cannot be registered but can be used with `Hook GHM`). The trade-off is that it requires admin privileges.

Unless you have special needs, prefer `Register GHM`. If you use `Hook GHM`, you should explain to users why admin privileges are needed and what the hotkeys do.

### Why does the library maintain extra threads?

---

This project decouples many modules, and some need their own threads.

For example, in addition to the GHM worker thread, `Hook GHM` needs a separate thread for the keyboard hook module. If you need deep customization, you can tailor it further.

### Does it support Wayland?

---

Only `Hook GHM`, and it requires admin privileges.

### Can `Register GHM` be used under Wayland?

---

No. On **Linux**, `Register GHM` depends on **X11**.

## Thread Safety

`Global Hotkey Manager` is a singleton, and its internal operations are thread-safe. This means you can call its member functions from different threads. However, most member functions **must not** be called from the manager’s own worker thread (for users, that is the thread where hotkey callbacks run). This is because these functions usually wait for operations to complete and return a success status. Calling them from the worker thread will deadlock.

## 🔔 Notes

- On macOS, `Register GHM` does not support console programs.

- Stopping the GHM service, registering, unregistering, and replacing hotkeys must only be performed after the GHM is initialized.

- Do not stop the service or register/unregister/replace hotkeys from the worker thread (i.e., the callback thread)!

- Avoid registering potentially invalid hotkeys. This is undefined behavior and may lead to unexpected results. The library does not validate such operations; it is up to the user.

- Hotkey callbacks run in the GHM worker thread, so they should not perform heavy tasks to avoid blocking. Use threads, async mechanisms, or message queues (e.g., Qt signals/slots).

- On **Windows**, when using `Hook GHM`, ensure callback execution time stays within the limit.

    *(See [Windows LowLevelKeyboard](https://learn.microsoft.com/zh-cn/windows/win32/winmsg/lowlevelkeyboardproc) — the **Remarks** mention a **1000 ms** timeout.)*

- On **Windows**, when `Ctrl+Shift+ESC` opens Task Manager, Task Manager blocks **LowLevelKeyboardHook** messages. As a result, `Hook GHM` may receive the key-down event for `Ctrl+Shift+ESC` but sometimes not the key-up event (depending on press duration and whether Task Manager is in the foreground). Other special shortcuts (e.g., `Ctrl+Alt+Delete`) can cause the same issue. Design your program accordingly. (See build option `GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY`; enabling it attempts to avoid this issue.)

---

For certain reasons, commit history before version `1.4.0` has been cleared. Earlier history can be found in the public archive repo: [global hotkey old](https://github.com/jaderochan/global_hotkey_old).
