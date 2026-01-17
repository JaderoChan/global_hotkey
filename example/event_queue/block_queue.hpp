#ifndef BLOCK_QUEUE_HPP
#define BLOCK_QUEUE_HPP

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class Queue
{
public:
    void push(const T& item)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.emplace(item);
        if (queue_.size() == 1)
            cvHas_.notify_all();
    }

    T take()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        cvHas_.wait(lock, [this]() { return !queue_.empty(); });
        T item = queue_.front();
        queue_.pop();
        return item;
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        while (!queue_.empty())
            queue_.pop();
    }

private:
    std::mutex mtx_;
    std::condition_variable cvHas_;
    std::queue<T> queue_;
};

#endif // !BLOCK_QUEUE_HPP
