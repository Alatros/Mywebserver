#pragma once
#include<arpa/inet.h>

class NetAddress
{
    public:
    struct sockaddr_in address;
    socklen_t addr_len;
    NetAddress(const char *ip, int port);
    NetAddress();
    NetAddress(const NetAddress &other);
    NetAddress(NetAddress &&other);
    ~NetAddress();

    char* get_ip() const;
    int get_port() const;
};
