#include"Socket.h"
#include"Epoll.h"
#include"utils.h"
#include"NetAddress.h"
#include<stdio.h>
#include <unistd.h>
#include <errno.h>
#include<memory>

#define READ_BUF_SIZE 1024

void handle_read_event(int);

int main(){
    std::unique_ptr<Socket> ser_sock(new Socket());
    std::unique_ptr<NetAddress> ser_addr_ptr(new NetAddress("127.0.0.1",8888));
    ser_sock->bind(*ser_addr_ptr);
    ser_sock->listen();
    std::unique_ptr<Epoll> ep(new Epoll());
    ser_sock->set_nonblock();
    ep->add_fd(ser_sock->get_fd(), EPOLLIN|EPOLLET);//边缘触发
    while(1){
        std::vector<epoll_event> events = ep->wait();
        int n = events.size();
        for(int i = 0;i < n; i++){
            if(events[i].data.fd == ser_sock->get_fd()){//新客户端连接
                NetAddress *clt_addr = new NetAddress();
                Socket *clt_sock = new Socket(ser_sock->accept(*clt_addr));
                printf("new client: %s:%d\n", clt_addr->get_ip(), clt_addr->get_port());
                clt_sock->set_nonblock();
                ep->add_fd(clt_sock->get_fd(), EPOLLIN|EPOLLET);
                delete clt_addr;
            }
            else if(events[i].events & EPOLLIN){//客户端发送数据
                handle_read_event(events[i].data.fd);
            }
        }
    }
    return 0;
}


void handle_read_event(int fd){
    char buf[READ_BUF_SIZE];
    while(1){
        ssize_t read_size = read(fd, buf, READ_BUF_SIZE);
        if(read_size>0){
            printf("read %ld bytes from client %d: %s\n", read_size, fd, buf);
            write(fd, buf, READ_BUF_SIZE);
        }
        else if(read_size == 0){//客户端关闭
            printf("client %d closed\n", fd);
            close(fd);
            break;
        }
        else if(read_size == -1 && ((errno == EAGAIN)||(errno == EWOULDBLOCK))){//缓冲区无数据
            printf("continue reading client %d \n", fd);
            break;
        }
        else if(read_size==-1 && (errno == EINTR)){//被信号中断
            printf("client %d interrupted by signal\n", fd);
            continue;
        }
        else{//其他错误
            printf("client %d read error\n", fd);
            close(fd);
            break;
        }
    }
    
}