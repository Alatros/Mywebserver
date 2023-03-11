#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<vector>
#include<functional>
#include<future>

class ThreadPool{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable condition;
    bool stop;
public:
    ThreadPool(size_t = std::thread::hardware_concurrency());
    ~ThreadPool();

    template<class F, class... Args>
    auto addTask(F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>{
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(this->mutex);
            if (this->stop) {
                throw std::runtime_error("ThreadPool is stopped");
            }
            this->tasks.emplace([task](){(*task)();});
        }
        this->condition.notify_one();
        return res;
    }
};

