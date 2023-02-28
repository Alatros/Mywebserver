#include"Socket.h"
#include"utils.h"
#include<unistd.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<cstdio>

Socket::Socket():fd(socket(AF_INET, SOCK_STREAM, 0))//创建套接字 ipv4 tcp 自动推导协议

{
    errorif(fd == -1, "Socket create error");
}

Socket::Socket(int fd):fd(fd)
{
    errorif(fd == -1, "Socket create error");
}

Socket::~Socket()
{
    printf("Socket destructor called\n");
    if(fd != -1)
    {
        close(fd);
        fd =-1;
    }
}


void Socket::bind(const NetAddress &addr)
{
    errorif(::bind(fd, (sockaddr*)&(addr.address), addr.addr_len) == -1, "Socket bind error");
}

void Socket::listen()
{
    errorif(::listen(fd, SOMAXCONN) == -1, "Socket listen error");
}

void Socket::set_nonblock(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

int Socket::accept(NetAddress &addr)
{
    int clt_fd = ::accept(fd, (sockaddr*)&(addr.address), &(addr.addr_len));
    errorif(clt_fd == -1, "Socket accept error");
    return clt_fd;
}

int Socket::get_fd() const
{
    return fd;
}

int Socket::connect(const NetAddress &addr)
{
    int ret = ::connect(fd, (sockaddr*)&(addr.address), addr.addr_len);
    errorif(ret == -1, "Socket connect error");
    return ret;
}