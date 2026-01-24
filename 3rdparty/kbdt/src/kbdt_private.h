#ifndef KBDT_PRIVATE_H
#define KBDT_PRIVATE_H

#include <kbdt/kbdt.h>

void set_run_success();

void set_run_fail(int error_code);

// The following functions have platform-specific implementations.

/**
 * @brief Platform-specific initialization before the worker thread starts.
 * @note This function is called before the worker thread begins execution.
 * @return 0 on success, non-zero error code on failure.
 */
int do_before_start_work();

/**
 * @brief Platform-specific cleanup before the worker thread terminates.
 * @note This function is called before the worker thread exits.
 * @note The worker thread will only exit after this function returns.
 * @return 0 on success, non-zero error code on failure.
 */
int do_before_end_work();

/**
 * @brief Platform-specific main work function.
 * @attention This function must call either `set_run_success()` or `set_run_fail()`
 * to indicate whether the work completed successfully.
 */
void work();

/**
 * @brief Platform-specific implementation to set the keyboard event handler.
 * @param handler Function pointer to handle keyboard events.
 * @return 0 on success, non-zero error code on failure.
 */
int set_event_handler_p(keyboard_event_handler handler);

/**
 * @brief Platform-specific implementation to send keyboard events.
 * @param events Array of keyboard events to send.
 * @param event_count Number of events in the array.
 * @return Number of events successfully sent.
 */
size_t send_events_p(struct keyboard_event* events, size_t event_count);

#endif // !KBDT_PRIVATE_H
