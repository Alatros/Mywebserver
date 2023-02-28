#define MAX_EVENTS 1000

#include"Epoll.h"
#include"utils.h"
#include<unistd.h>

Epoll::Epoll()
{
    epoll_fd = epoll_create1(0);
    errorif(epoll_fd == -1, "epoll_create1 error");
    events = new epoll_event[MAX_EVENTS];
}

Epoll::~Epoll()
{
    if(epoll_fd != -1)
    {
        close(epoll_fd);
        epoll_fd = -1;
    }   
}

void Epoll::add_fd(int fd, uint32_t events)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = events;
    errorif(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1, "epoll_ctl error");
}

std::vector<epoll_event> Epoll::wait(int timeout)
{
    std::vector<epoll_event> active_events;
    int n = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout);
    errorif(n == -1, "epoll_wait error");
    for(int i = 0; i < n; i++)
    {
        active_events.push_back(events[i]);
    }
    return active_events;
}