#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i) { //enable threads
        this->threads.emplace_back([this] {
            for (;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->mutex);
                    this->condition.wait(lock, [this] {
                        return this->stop || !this->tasks.empty();//wait for tasks when not stop and tasks is empty
                    });
                    if (this->stop && this->tasks.empty()) {
                        return;
                    }
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->stop = true;
    }
    this->condition.notify_all();//notify all threads
    for (std::thread &thread : this->threads) {
        thread.join();//wait for threads to finish
    }
}

