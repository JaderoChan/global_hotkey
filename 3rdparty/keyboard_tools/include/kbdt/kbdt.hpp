// Repository: https://github.com/JaderoChan/keyboard_tools
// Author: 頔珞JaderoChan

// MIT License
//
// Copyright (c) 2025 頔珞JaderoChan
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef KBDT_HPP
#define KBDT_HPP

#include <vector>

#include <kbdt/config.hpp>

#include "defines.hpp"
#include "types.hpp"

namespace kbdt
{

class KBDT_API KeyboardToolsManager
{
public:
    static KeyboardToolsManager& getInstance();

    /**
     * @brief Run the keyboard tools service.
     * @return KBDT_RC_SUCCESS on success, error code on failure.
     */
    int run();

    /**
     * @brief Stop the keyboard tools service.
     * @attention Do not call this function from within the event handler thread.
     * @return KBDT_RC_SUCCESS on success, error code on failure.
     */
    int stop();

    /**
     * @brief Set the key event handler callback.
     * @param handler Function pointer to handle key events.
     * @note It only be set while service is running.
     * @return KBDT_RC_SUCCESS on success, error code on failure.
     */
    int setEventHandler(KeyEventHandler handler);

    /**
     * @brief Check if the keyboard tools service is running.
     * @return True if the service is running, false otherwise.
     */
    bool isRunning() noexcept;

private:
    KeyboardToolsManager() = default;
    ~KeyboardToolsManager() = default;
    KeyboardToolsManager(const KeyboardToolsManager&) = delete;
    KeyboardToolsManager& operator=(const KeyboardToolsManager&) = delete;
};

/**
 * @brief Send multiple key events.
 * @note Events sent through this function will still be received by the event handler.
 * @param events List of key events to send.
 * @return Number of events successfully sent.
 */
size_t sendEvents(const std::vector<KeyEvent>& events);

/**
 * @brief Send a single key event.
 * @note Events sent through this function will still be received by the event handler.
 * @param event Key event to send.
 * @return True if the event been sent successfully, false otherwise.
 */
bool sendEvent(const KeyEvent& event);

/**
 * @brief Check whether the propagation blocking of the key event is supported.
 */
constexpr bool isEventPropagationBlockSupported()
{
#ifdef KBDT_WIN
    return true;
#elif defined(KBDT_MAC)
    return true;
#elif defined(KBDT_LINUX)
    return false;
#else
    return false;
#endif // KBDT_WIN
}

} // namespace kbdt

#endif // !KBDT_HPP
