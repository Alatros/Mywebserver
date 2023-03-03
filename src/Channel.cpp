#include "Channel.h"

#include <unistd.h>
#include <utility>
#include "Epoll.h"
#include "Eventloop.h"

Channel::Channel(Eventloop *_el, int _fd) : el(_el), fd(_fd), events(0), revents(0), inEpoll(false){

}

Channel::~Channel()
{
    if(fd!=-1){
        close(fd);
        fd=-1;
    }
}

void Channel::enableReading(){
    events = EPOLLIN | EPOLLET;
    el->updateChannel(this);
}

int Channel::getFd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}
uint32_t Channel::getRevents(){
    return revents;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(){
    inEpoll = true;
}

void Channel::setRevents(uint32_t _ev){
    revents = _ev;
}

void Channel::setCallback(std::function<void()>& _cb){
    callback = std::move(_cb);
}

void Channel::handleEvent(){
    callback();
}