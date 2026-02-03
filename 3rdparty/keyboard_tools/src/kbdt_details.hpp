#ifndef KBDT_DETAILS_HPP
#define KBDT_DETAILS_HPP

#include <vector>

#include <kbdt/defines.hpp>
#include <kbdt/types.hpp>

namespace kbdt
{

void setRunSuccess();

void setRunFail(int errorCode);

namespace details
{

// The following functions have platform-specific implementations.

/**
 * @brief Platform-specific initialization before the worker thread starts.
 * @note This function is called before the worker thread begins execution.
 * @return KBDT_RC_SUCCESS on success, error code on failure.
 */
int initialize();

/**
 * @brief Platform-specific cleanup before the worker thread terminates.
 * @note This function is called before the worker thread exits.
 * @note The worker thread will only exit after this function returns.
 * @return KBDT_RC_SUCCESS on success, error code on failure.
 */
int finalize();

/**
 * @brief Platform-specific implementation to set the key event handler.
 * @param handler Function pointer to handle key events.
 * @return KBDT_RC_SUCCESS on success, error code on failure.
 */
int setEventHandler(KeyEventHandler handler);

/**
 * @brief Platform-specific main work function.
 * @attention This function must call either `setRunSuccess()` or `setRunFail()`
 * to indicate whether the work completed successfully.
 */
void work();

/**
 * @brief Platform-specific implementation to send key events.
 * @param events List of key events to send.
 * @return Number of events successfully sent.
 */
size_t sendEvents(const std::vector<KeyEvent>& events);

} // namespace details

} // namespace kbdt

#endif // !KBDT_DETAILS_HPP
