#pragma once

#include "Epoll.h"
#include "Channel.h"

class ThreadPool;
class Eventloop{
private:
    Epoll* ep;
    bool quit;
    // ThreadPool *pool;
public:
    Eventloop();
    ~Eventloop();
    void loop();
    void updateChannel(Channel*);

    void addTask(std::function<void()>&);
};