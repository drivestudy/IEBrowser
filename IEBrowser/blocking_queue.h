#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>

template <typename T>
class BlockingQueue
{
public:
    BlockingQueue()
    {
    }
    ~BlockingQueue()
    {
    }

    void Push(T value)
    {
        std::lock_guard<std::mutex> lock_guard(mutex_);
        queue_.push_back(std::move(value));
        not_empty_.notify_one();
    }

    T Take()
    {
        std::unique_lock<std::mutex> lock_guard(mutex_);

        while (queue_.empty())
        {
            not_empty_.wait(lock_guard);
        }

        T front(std::move(queue_.front()));
        queue_.pop_front();

        return front;
    }

private:
    std::deque<T> queue_;
    std::mutex mutex_;
    std::condition_variable not_empty_;
};