
#ifndef THREADSAFEQUEUE_HPP
    #define THREADSAFEQUEUE_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace tdl {

    template <typename T>
    class ThreadSafeQueue {
        public:
            void push(const T &value) {
                std::unique_lock<std::mutex> lock(_mutex);
                if (_queue.size() > 10) {
                    _queue.pop();
                }
                _queue.push(value);
                lock.unlock();
                _condition.notify_one();
            }

            bool poll(T& data) {
                std::unique_lock<std::mutex> lock(_mutex);
                if (_queue.empty()) return false;
                data = _queue.front();
                _queue.pop();
                return true;
            }

            void waitAndPop(T& data) {
                std::unique_lock<std::mutex> lock(_mutex);
                _condition.wait(lock, [this] { return !_queue.empty(); });
                data = _queue.front();
                _queue.pop();
            }

        std::atomic<bool> read = true;

        private:
            std::queue<T> _queue;
            std::mutex _mutex;
            std::condition_variable _condition;
    };
}

#endif //THREADSAFEQUEUE_HPP
