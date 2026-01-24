// Repository: https://github.com/JaderoChan/keyboard_tool
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

#ifndef KBDT_H
#define KBDT_H

#include <stdbool.h>
#include <stddef.h>

#include "config.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifdef _MSC_VER
    #ifdef KBDT_BUILD_SHARED
        #define KBDT_API __declspec(dllexport)
    #elif defined(KBDT_SHARED)
        #define KBDT_API __declspec(dllimport)
    #else
        #define KBDT_API
    #endif // KBDT_BUILD_SHARED
#else
    #define KBDT_API __attribute__((visibility("default")))
#endif // _WIN32

/// @brief Return code for successful operation.
#define KBDT_RC_SUCCESS     0

/**
 * @brief Keyboard event types.
 */
enum keyboard_event_type
{
    KBDET_RELEASED, ///< Key released event
    KBDET_PRESSED   ///< Key pressed event
};

/**
 * @brief Keyboard event structure.
 */
struct keyboard_event
{
    enum keyboard_event_type type;   ///< Type of the keyboard event
    int native_key;                  ///< Native key code
};

/**
 * @brief Keyboard event handler callback type.
 */
typedef void (*keyboard_event_handler)(struct keyboard_event*);

/**
 * @brief Start the keyboard event monitoring service.
 * @return KBDT_RC_SUCCESS on success, error code on failure.
 */
KBDT_API int kbdt_start();

/**
 * @brief Stop the keyboard event monitoring service.
 * @attention Do not call this function from within the event handler thread.
 * @return KBDT_RC_SUCCESS on success, error code on failure.
 */
KBDT_API int kbdt_stop();

/**
 * @brief Set the keyboard event handler callback.
 * @param handler Function pointer to handle keyboard events.
 * @return KBDT_RC_SUCCESS on success, error code on failure.
 */
KBDT_API int kbdt_set_event_handler(keyboard_event_handler handler);

/**
 * @brief Send multiple keyboard events.
 * @note Events sent through this function bypass the event handler.
 * @param events Array of keyboard events to send.
 * @param event_count Number of events in the array.
 * @return Number of events successfully sent.
 */
KBDT_API size_t kbdt_send_events(struct keyboard_event* events, size_t event_count);

/**
 * @brief Send a single keyboard event.
 * @note Events sent through this function bypass the event handler.
 * @param event Keyboard event to send.
 * @return Number of events successfully sent (0 or 1).
 */
KBDT_API size_t kbdt_send_event(struct keyboard_event event);

/**
 * @brief Add multiple events to the event buffer.
 * @param events Array of keyboard events to buffer.
 * @param event_count Number of events in the array.
 * @return Number of events successfully added to the buffer.
 */
KBDT_API size_t kbdt_add_events_to_buf(struct keyboard_event* events, size_t event_count);

/**
 * @brief Add a single event to the event buffer.
 * @param event Keyboard event to buffer.
 * @return Number of events successfully added to the buffer (0 or 1).
 */
KBDT_API size_t kbdt_add_event_to_buf(struct keyboard_event event);

/**
 * @brief Clear all events from the event buffer.
 */
KBDT_API void kbdt_clear_event_buf();

/**
 * @brief Send all events currently in the buffer without clearing it.
 * @note Events sent through this function bypass the event handler.
 * @return Number of events successfully sent.
 */
KBDT_API size_t kbdt_send_event_buf();

/**
 * @brief Send all events in the buffer and then clear it.
 * @note Events sent through this function bypass the event handler.
 * @return Number of events successfully sent (flushed).
 */
KBDT_API size_t kbdt_flush_event_buf();

/**
 * @brief Check if the keyboard event monitoring service is running.
 * @return true if the service is running, false otherwise.
 */
KBDT_API bool kbdt_is_running();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !KBDT_H
