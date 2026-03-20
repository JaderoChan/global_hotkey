#include "register_ghm_private_x11.hpp"

#include <cstdint>
#include <poll.h>           // poll
#include <sys/eventfd.h>    // eventfd
#include <unistd.h>         // read, write, close

#include <global_hotkey/return_code.hpp>
#include <key/key_private_x11.hpp>

namespace gbhk
{

int ErrorHandler::ec = RC_SUCCESS;
XErrorHandler ErrorHandler::prevXErrorHandler_;

ErrorHandler::ErrorHandler()
{
    ec = RC_SUCCESS;
    prevXErrorHandler_ = XSetErrorHandler(&ErrorHandler::handleError);
}

ErrorHandler::~ErrorHandler()
{
    XSetErrorHandler(prevXErrorHandler_);
}

int ErrorHandler::handleError(Display* display, XErrorEvent* error)
{
    if (error->error_code != Success)
        ec = error->error_code;
    return ec;
}

std::unordered_map<int, int> RegisterGHMPrivateX11::keycodeToKeysym_;

RegisterGHMPrivateX11::RegisterGHMPrivateX11() {}

RegisterGHMPrivateX11::~RegisterGHMPrivateX11() { stop(); }

int RegisterGHMPrivateX11::initialize()
{
    eventFd_ = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    if (eventFd_ == -1)
        return errno;
    return RC_SUCCESS;
}

int RegisterGHMPrivateX11::stopWork()
{
    {
        std::lock_guard<std::mutex> locker(eventsMtx);
        events_.push_back(ET_EXIT);
    }

    uint64_t wakeup = 1;
    auto wsize = write(eventFd_, &wakeup, 8);
    if (wsize != 8)
    {
        std::lock_guard<std::mutex> locker(eventsMtx);
        events_.pop_back();
        return errno;
    }
    return RC_SUCCESS;
}

void RegisterGHMPrivateX11::work()
{
    Display* display = nullptr;
    {
        ErrorHandler eh;
        display = XOpenDisplay(nullptr);
        if (eh.ec != RC_SUCCESS)
        {
            setRunFail(eh.ec);
            return;
        }
    }

    int x11Fd = ConnectionNumber(display);
    struct pollfd pollFds[2] = {0};
    pollFds[0] = struct pollfd{x11Fd, POLLIN};
    pollFds[1] = struct pollfd{eventFd_, POLLIN};

    setRunSuccess();
    KeyCombination prevKc;
    KeyCombination currKc;
    XEvent event = {0};

    bool shouldClose = false;
    while (!shouldClose)
    {
        int ret = poll(pollFds, 2, -1);
        if (ret == -1)
            continue;

        // XEvent was detected.
        if (pollFds[0].revents & POLLIN)
        {
            while (XPending(display))
            {
                XNextEvent(display, &event);
                if (event.type == KeyPress)
                {
                    auto mod = modifiersFromX11Modifiers(event.xkey.state);
                    auto keysym = keycodeToKeysym_[event.xkey.keycode];
                    auto key = keyFromX11Keysym(keysym);
                    currKc = {mod, key};
                }
                else
                {
                    currKc = {};
                }
            }
            tryInvoke(prevKc, currKc);
            prevKc = currKc;
        }

        // Custom event was detected.
        if (pollFds[1].revents & POLLIN)
        {
            uint64_t counting;
            ssize_t rsize = read(eventFd_, &counting, 8);
            if (rsize == 8)
            {
                for (uint64_t i = 0; i < counting && !shouldClose; ++i)
                {
                    EventType et;
                    {
                        std::lock_guard<std::mutex> locker(eventsMtx);
                        if (events_.empty())
                            break;
                        et = events_.front();
                        events_.pop_front();
                    }

                    switch (et)
                    {
                        case ET_EXIT:
                            shouldClose = true;
                            break;
                        case ET_REGISTER:
                        {
                            std::lock_guard<std::mutex> locker(regUnregRcKcMtx_);
                            regUnregRc_ = nativeRegisterHotkey(display);
                        }
                            regUnregRcCv_.notify_one();
                            break;
                        case ET_UNREGISTER:
                        {
                            std::lock_guard<std::mutex> locker(regUnregRcKcMtx_);
                            regUnregRc_ = nativeUnregisterHotkey(display);
                        }
                            regUnregRcCv_.notify_one();
                            break;
                    }
                }
            }
        }
    }

    XCloseDisplay(display);

    close(eventFd_);
    eventFd_ = -1;
    events_.clear();

    ErrorHandler::ec = RC_SUCCESS;

    regUnregRc_ = -2;
    regUnregKc_ = KeyCombination();
}

int RegisterGHMPrivateX11::registerHotkeyImpl(const KeyCombination& kc, bool autoRepeat)
{
    {
        std::lock_guard<std::mutex> locker(regUnregRcKcMtx_);
        regUnregRc_ = -1;
        regUnregKc_ = kc;
    }

    {
        std::lock_guard<std::mutex> locker(eventsMtx);
        events_.push_back(ET_REGISTER);
    }

    uint64_t wakeup = 1;
    auto wsize = write(eventFd_, &wakeup, 8);
    if (wsize != 8)
    {
        std::lock_guard<std::mutex> locker(eventsMtx);
        events_.pop_back();
        return errno;
    }

    std::unique_lock<std::mutex> locker(regUnregRcKcMtx_);
    regUnregRcCv_.wait(locker, [this]() { return regUnregRc_ != -1; });
    return regUnregRc_;
}

int RegisterGHMPrivateX11::unregisterHotkeyImpl(const KeyCombination& kc)
{
    {
        std::lock_guard<std::mutex> locker(regUnregRcKcMtx_);
        regUnregRc_ = -1;
        regUnregKc_ = kc;
    }

    {
        std::lock_guard<std::mutex> locker(eventsMtx);
        events_.push_back(ET_UNREGISTER);
    }

    uint64_t wakeup = 1;
    auto wsize = write(eventFd_, &wakeup, 8);
    if (wsize != 8)
    {
        std::lock_guard<std::mutex> locker(eventsMtx);
        events_.pop_back();
        return errno;
    }

    std::unique_lock<std::mutex> locker(regUnregRcKcMtx_);
    regUnregRcCv_.wait(locker, [this]() { return regUnregRc_ != -1; });
    return regUnregRc_;
}

int RegisterGHMPrivateX11::nativeRegisterHotkey(Display* display)
{
    ErrorHandler eh;

    auto keysym = keyToX11Keysym(regUnregKc_.key());
    auto keycode = XKeysymToKeycode(display, keysym);
    keycodeToKeysym_[keycode] = keysym;
    auto mod = modifiersTox11Modifiers(regUnregKc_.modifiers());
    XGrabKey(display, keycode, mod, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
    XSync(display, False);

    return eh.ec;
}

int RegisterGHMPrivateX11::nativeUnregisterHotkey(Display* display)
{
    ErrorHandler eh;

    auto keysym = keyToX11Keysym(regUnregKc_.key());
    auto keycode = XKeysymToKeycode(display, keysym);
    keycodeToKeysym_.erase(keycode);
    auto mod = modifiersTox11Modifiers(regUnregKc_.modifiers());
    XUngrabKey(display, keycode, mod, DefaultRootWindow(display));
    XSync(display, False);

    return eh.ec;
}

void RegisterGHMPrivateX11::tryInvoke(const KeyCombination& prevKc, const KeyCombination& currKc) const
{
    auto pair = getPairValue(currKc);
    const auto& autoRepeat = pair.first;
    const auto& fn = pair.second;
    bool shouldInvoke = fn && (currKc != prevKc || autoRepeat);
    if (shouldInvoke)
        fn();
}

} // namespace gbhk
