#pragma once
#include <sys/epoll.h>
#include<functional>

class Eventloop;
class Channel
{
private:
    Eventloop *el;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;

    std::function<void()> callback;
public:
    Channel(Eventloop *, int);
    ~Channel();

    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();

    void setRevents(uint32_t);

    void handleEvent();
    void setCallback(std::function<void()>&);
};

