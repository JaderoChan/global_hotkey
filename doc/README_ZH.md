# Global Hotkey

**[English](README_EN.md) | 简体中文**

## 🚀 特点

- 独立！

    不依赖其他库与框架！

- 用于任意程序！

    甚至是控制台程序！

- 跨平台！

    **Windows**、**MacOS**、**Linux**（*Ubuntu*与*Debian*已测试）不在话下！

- 广泛兼容！

    **Qt**？**MFC**？没问题！

- 易于使用！

    参见示例代码。

- 监听式热键 (`Hook GHM`)！

    需要实现~~后台监听木马~~某些特殊的功能？

## 依赖

[Keyboard Tools](https://github.com/JaderoChan/keyboard_tools)

## 🔧 如何编译？

1. 直接运行根目录下的`install`脚本即可一键安装，可用选项如下：

    - [--build-example]（与`GLOBAL_HOTKEY_BUILD_EXAMPLE=ON`功能相同）
    - [--build-example-use-hook]（与`GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK=ON`功能相同）（**Wayland**下不可用，因为**Wayland**下此选项是必须的）
    - [--prefix \<path\>]（指定安装路径）

    例如`install --build-example --prefix install`将会安装库至 *./install* 目录下，并构建示例程序。

2. 项目使用`CMake`进行组织，仅需下面几行脚本即可编译使用！

    ```shell
    git clone --recurse-submodules https://github.com/JaderoChan/global_hotkey.git
    cd global_hotkey
    cmake -B build
    cmake --build build
    ```

### ⚙️ 编译选项

---

- `GLOBAL_HOTKEY_BUILD_SHARED` 指定是否生成动态库，默认为`OFF`。若启用，在项目中使用动态库时定义`GLOBAL_HOTKEY_SHARED`宏以获得更好的性能（仅**Windows**）。

- `GLOBAL_HOTKEY_DISABLE_REGISTER` 指定是否禁用`注册式热键 (Register GHM)`，默认为`OFF`。

- `GLOBAL_HOTKEY_DISABLE_HOOK` 指定是否禁用`监听式热键 (Hook GHM)`），默认为`OFF`。

- `GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY` 此选项仅针对Windows平台上的Hook GHM。如果启用此选项，当`Ctrl+Shift+ESC`与`Ctrl+Alt+Delete`热键被触发时将使用一些技巧尝试防止异常的程序行为。默认启用。

- `GLOBAL_HOTKEY_BUILD_EXAMPLE` 是否生成示例程序，默认值取决于项目是否为主项目。

- `GLOBAL_HOTKEY_BUILD_EXAMPLE_USE_HOOK` 指定示例程序使用的热键类型，默认为`OFF`（即在示例程序中使用`Register GHM`）。

- `GLOBAL_HOTKEY_EXAMPLE_BUILD_SIMPLE` 指定是否生成`simple`示例程序，默认`ON`。

- `GLOBAL_HOTKEY_EXAMPLE_BUILD_EVENT_QUEUE` 指定是否生成`event quque`实例程序，默认为`ON`。

## 🚩 如何使用？

1. 通过`GlobalHotkeyManager::getInstance`接口获取`热键管理器 (GHM)`对象实例。

2. 通过`GlobalHotkeyManager::run`接口运行`GHM`服务。

3. 通过相应的接口增加、移除或替换热键。

4. 热键被触发时将执行对应的回调函数。

5. 通过`GlobalHotkeyManager::stop`接口终止`GHM`服务

---

下面展示了一个基本流程的示例代码

```cpp
GlobalHotkeyManager& ghm = RegisterGlobalHotkeyManager::getInstance();  // 获取 注册式热键管理器 实例对象。
ghm.run();   // 启动热键管理器服务。

KeyCombination hotkey1(CTRL, 'G');
KeyCombination hotkey2(CTRL, 'H');
KeyCombination hotkey3(CTRL, 'M');
ghm.registerHotkey(hotkey1, &callback);                        // 绑定一个回调函数。
ghm.registerHotkey(hotkey2, [=]() { if(isOk) emitSignal(); }); // 绑定一个Lambda函数。在热键触发且条件为真时发射一个信号。
ghm.registerHotkey(hotkey3, [=]() { printf("Hello world!") }); // 仅仅打印一个消息。

// 主循环。
while (!shouldClose)
{
    // Do Something.
}

ghm.stop(); // 释放热键管理器。
```

## 💡 示例

[简单示例](example/simple/main.cpp)

[事件队列示例](example/event_queue/main.cpp)

## ❓ 答疑

### 许可证是什么？

---

本库使用MIT许可证，这意味着你可以拿它做任何事情。尽管不是必须，但在程序中进行署名是值得赞誉的！

### `Register GHM` 和 `Hook GHM`有什么区别？

---

`Register GHM`由操作系统或桌面环境进行维护，它可能会与其他应程序的全局热键发生冲突，而且通常来讲它不需要管理员权限。

`Hook GHM`完全基于`Hook`之类的键盘监听技术，并于此实现热键逻辑，所以它不会与其他应用程序的全局热键发生冲突，并且可使用的热键更为宽泛（例如在**Windows**下，`Win + T`是不可被注册为热键的，但通过`Hook GHM`可以做到），代价是它需要管理员权限。

除某些特殊需求外，应该尽量使用`Register GHM`，如果使用`Hook GHM`，你应该向用户阐明需要管理员权限的原因以及热键的具体用途。

### 为什么库需要维护额外的线程？

---

本项目对许多模块进行了解耦，而这些模块有时需要各维护一个线程。

例如`HooK GHM`除`热键管理器 (GHM)`自身的工作线程外，还需要额外维护一个`键盘监听 (Keyboard Hook)`模块的线程，如果你的程序需要进行深度的定制，你可以在此基础上进行更针对性的修改。

### 支持Wayland窗口系统吗？

---

仅支持`Hook GHM`，而`Hook GHM`需要管理员权限才可以正常工作。

### Wayland窗口系统下可以使用`Register GHM`吗？

---

不可以。**Linux**下的`Register GHM`依赖于**X11**。

## 线程安全

`Global Hotkey Manager`是一个单例类，其内部操作是线程安全的，这意味着你可以在不同的线程调用`GHM`的成员函数。但值得注意的是，`GHM`大部分成员函数都不能够在自身的工作线程中被调用（对使用者而言，最关键的点在于，热键的回调函数是在工作线程中被执行的），之所以会这样，是因为`GHM`大多数函数都需要等待操作完成并返回一个值以指示操作是否成功，如果在工作线程中进行这个操作的话会产生死锁，那样函数将永远不会返回。

## 🔔 注意

- MacOS系统下的`Register GHM`暂不支持。

- 中止`GHM`服务、注册热键、注销热键和替换热键等操作，必须在对应的`GHM`初始化之后才可进行！

- 不要在工作线程中（对于使用者而言，这是热键被触发时回调函数执行的线程）进行中止`GHM`服务、注册热键、注销热键和替换热键等操作！

- 避免注册一个可能无效的热键，这是未定义行为，可能造成非预期结果。本库对此类操作不做安全性检查，这些操作应该由用户决定！

- 热键被触发时，其回调函数将会在`GHM`的工作线程中运行，所以热键的回调函数不应执行繁重的任务，以免出现工作线程阻塞，合理的做法是正确使用线程、异步机制或消息队列（例如**Qt**中的信号槽）。

- 在**Windows**平台下使用`Hook GHM`时，应保证回调函数的执行时间在限定范围内。

    *（详细信息参见 [Windows LowLevelKeyboard](https://learn.microsoft.com/zh-cn/windows/win32/winmsg/lowlevelkeyboardproc) ，其**Remarks**中提到**Timeout**时间为**1000毫秒**）*

- 在**Windows**平台下使用`Hook GHM`时，当用户按下`Ctrl+Shift+ESC`快捷键呼出**任务管理器**后，由于**任务管理器**会阻止**LowLevelKeyboardHook**消息，所以会导致`Hook GHM`能够接收到`Ctrl+Shift+ESC`的按键按下消息，但有概率无法收到按键释放消息（取决于你的按压时长与**任务管理器**是否已经成为焦点窗口）。除`Ctrl+Shift+Esc`外，还存在一些其他的特殊快捷键（如`Ctrl+Alt+Delete`）也会导致这个问题。在设计相关程序时应该着重注意。（参见编译选项`GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY`，启用此选项将尝试避免此问题）

---

出于某些原因，本库1.4.0版本之前的commit记录已被清空，此前的历史commit可以通过[global hotkey old](https://github.com/jaderochan/global_hotkey_old)公共存档库进行查看。
