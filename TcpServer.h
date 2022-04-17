#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "reactorConmon.h"
#include "TcpTool.h"
#include "Epoller.h"

class Epoller;

// 相当于Reactor
class TcpServer
{
public:
    TcpServer(int port);
    ~TcpServer();

public:
    void run(); // 启动服务器


// 回调函数处理
private:
    void acceptConnection();  // 接收连接
    void closeConnection(TcpTool *tool);
    //处理读写
    void handleWrite(TcpTool *tool);
    void handleRead(TcpTool *tool);


    int createListenFd(int port); // 创建监听描述符



private:
    using EpollPtr = std::unique_ptr<Epoller>;  // 采用智能指针管理epoller类，独占资源
    using ListenTcpToolPtr = std::unique_ptr<TcpTool>;
    int m_port;
    int m_listenFd;
    ListenTcpToolPtr m_listenTool;    //
    EpollPtr m_epoll;                 // 只能有一个epoll实例，所以使用unique_ptr  自动释放
};





#endif // _TCP_SERVER_H_