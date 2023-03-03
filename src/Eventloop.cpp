#include"Eventloop.h"

Eventloop::Eventloop(){
    ep = new Epoll();
    quit = false;
}

Eventloop::~Eventloop(){
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