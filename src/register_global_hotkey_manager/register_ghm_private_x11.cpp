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

RegisterGHMPrivateX11::RegisterGHMPrivateX11() :
    regUnregRc_(0),
    regUnregKc_(KeyCombination())
{}

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
    EventType et = ET_EXIT;
    auto wsize = write(eventFd_, &et, 8);
    if (wsize != 8)
        return errno;
    return RC_SUCCESS;
}

void RegisterGHMPrivateX11::work()
{
    Display* display = NULL;
    {
        ErrorHandler eh;
        display = XOpenDisplay(NULL);
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
    while (true)
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
            EventType et;
            auto rsize = read(eventFd_, &et, 8);
            if (rsize != 8)
                continue;

            if (et == ET_EXIT)
            {
                break;
            }
            else if (et == ET_REGISTER)
            {
                regUnregRc_ = nativeRegisterHotkey(display);
                cvRegUnregRc_.notify_one();
            }
            else if (et == ET_UNREGISTER)
            {
                regUnregRc_ = nativeUnregisterHotkey(display);
                cvRegUnregRc_.notify_one();
            }
        }
    }

    XCloseDisplay(display);
    close(eventFd_);

    ErrorHandler::ec = RC_SUCCESS;
    regUnregRc_ = 0;
    regUnregKc_ = KeyCombination();
    eventFd_ = -1;
}

int RegisterGHMPrivateX11::registerHotkeyImpl(const KeyCombination& kc, bool autoRepeat)
{
    regUnregRc_ = -1;
    regUnregKc_ = kc;
    EventType et = ET_REGISTER;
    auto wsize = write(eventFd_, &et, 8);
    if (wsize != 8)
        return errno;

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRegUnregRc_.wait(lock, [this]() { return regUnregRc_ != -1; });
    return regUnregRc_;
}

int RegisterGHMPrivateX11::unregisterHotkeyImpl(const KeyCombination& kc)
{
    regUnregRc_ = -1;
    regUnregKc_ = kc;
    EventType et = ET_UNREGISTER;
    auto wsize = write(eventFd_, &et, 8);
    if (wsize != 8)
        return errno;

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRegUnregRc_.wait(lock, [this]() { return regUnregRc_ != -1; });
    return regUnregRc_;
}

int RegisterGHMPrivateX11::nativeRegisterHotkey(Display* display)
{
    ErrorHandler eh;

    auto keysym = keyToX11Keysym(regUnregKc_.load().key());
    auto keycode = XKeysymToKeycode(display, keysym);
    keycodeToKeysym_[keycode] = keysym;
    auto mod = modifiersTox11Modifiers(regUnregKc_.load().modifiers());
    XGrabKey(display, keycode, mod, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
    XSync(display, False);

    return eh.ec;
}

int RegisterGHMPrivateX11::nativeUnregisterHotkey(Display* display)
{
    ErrorHandler eh;

    auto keysym = keyToX11Keysym(regUnregKc_.load().key());
    auto keycode = XKeysymToKeycode(display, keysym);
    auto mod = modifiersTox11Modifiers(regUnregKc_.load().modifiers());
    XUngrabKey(display, keycode, mod, DefaultRootWindow(display));
    XSync(display, False);

    return eh.ec;
}

void RegisterGHMPrivateX11::tryInvoke(const KeyCombination& prevKc, const KeyCombination& currKc) const
{
    auto pair = getPairValue(currKc);
    auto& autoRepeat = pair.first;
    auto& fn = pair.second;
    bool shouldInvoke = fn && (currKc != prevKc || autoRepeat);
    if (shouldInvoke)
        fn();
}

} // namespace gbhk
