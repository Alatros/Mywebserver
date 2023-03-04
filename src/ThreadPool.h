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

    void addTask(std::function<void()>);
};
