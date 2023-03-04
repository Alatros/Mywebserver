#include"Eventloop.h"
#include"ThreadPool.h"

Eventloop::Eventloop(){
    ep = new Epoll();
    pool = new ThreadPool();
    quit = false;
}

Eventloop::~Eventloop(){
    delete pool;
    delete ep;
}

void Eventloop::loop(){
    while(!quit){
        std::vector<Channel*> channels;
        channels = ep->poll();
        for(auto &channel: channels){
            channel->handleEvent();
        }
    }
}

void Eventloop::updateChannel(Channel *channel){
    ep->updateChannel(channel);
}

void Eventloop::addTask(std::function<void()>& task){
    pool->addTask(task);
}