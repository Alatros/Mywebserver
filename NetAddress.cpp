#include"NetAddress.h"
NetAddress::NetAddress():addr_len(sizeof(address))
{
    address.sin_family = AF_INET;
    address.sin_port = htons(0);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
}

NetAddress::NetAddress(const char *ip, int port):addr_len(sizeof(address))
{
    address.sin_family = AF_INET;//地址族 IPV4 ipv6为AF_INET6
    address.sin_port = htons(port);//端口号
    address.sin_addr.s_addr = inet_addr(ip);//ip地址
}

NetAddress::NetAddress(const NetAddress &other):addr_len(sizeof(address))
{
    address.sin_family = other.address.sin_family;
    address.sin_port = other.address.sin_port;
    address.sin_addr.s_addr = other.address.sin_addr.s_addr;
}
NetAddress::NetAddress(NetAddress &&other):addr_len(sizeof(address))
{
    address.sin_family = other.address.sin_family;
    address.sin_port = other.address.sin_port;
    address.sin_addr.s_addr = other.address.sin_addr.s_addr;
    other.address.sin_family = 0;
    other.address.sin_port = 0;
    other.address.sin_addr.s_addr = 0;
}
NetAddress::~NetAddress()
{
    address.sin_family = 0;
    address.sin_port = 0;
    address.sin_addr.s_addr = 0;
}

char* NetAddress::get_ip() const
{
    return inet_ntoa(address.sin_addr);
}

int NetAddress::get_port() const
{
    return ntohs(address.sin_port);
}