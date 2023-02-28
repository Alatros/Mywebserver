#pragma once
#include<sys/epoll.h>
#include<vector>

class Epoll
{
private:
    int epoll_fd;
    struct epoll_event *events;
    
public:
    Epoll();
    Epoll(const Epoll &other) = delete;
    Epoll(Epoll &&other) = delete;
    ~Epoll();
    void add_fd(int fd, uint32_t events);

    std::vector<epoll_event> wait(int timeout=-1);
};