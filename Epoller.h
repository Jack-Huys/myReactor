#ifndef _EPOLLER_H_
#define _EPOLLER_H_

#include "reactorConmon.h"
#include "TcpTool.h"

class TcpTool; // 前向

// IO多路复用 操作类  提供事件回调
class Epoller
{
public:
    Epoller();
    ~Epoller();

    // std::function是一个函数包装器，该函数包装器模板能包装任何类型的可调用实体，如普通函数，函数对象，lamda表达式、函数指针等 不可拷贝和移动
    // 用function来包裹回调函数
    using newConnectionCallback = std::function<void()>; // 定于了一种回调函数类型 void 
    using handleReadCallback = std::function<void(TcpTool*)>;
    using handleWriteCallback = std::function<void(TcpTool*)>;
    using closeConnectionCallback = std::function<void(TcpTool*)>;

    int add(int fd, TcpTool *tool, int events);
    int mod(int fd, TcpTool *tool, int events);
    int del(int fd, TcpTool *tool, int events);
    int wait(int timeouts); // 等待轮询返回的事件 可以设置超时时间

    // 事件处理
    void handleEvents(int listenFd, int eventsNum);

    //事件回调函数
    void setNewConnection(const newConnectionCallback &cb){
        m_connentioncb = cb;
    }
    void setCloseConnection(const closeConnectionCallback &cb){
        m_closecb = cb;
    }
    void setWriteCb(const handleWriteCallback &cb){
        m_writecb = cb;
    }
    void setReadCb(const handleReadCallback &cb){
        m_readcb = cb;
    }


private:
    using eventList = std::vector<struct epoll_event>; 
    eventList  m_events;  //事件集合

    int m_epollFd; // epoll fd

    newConnectionCallback m_connentioncb; // 回调函数包裹实例对象
    handleReadCallback  m_readcb;
    handleWriteCallback m_writecb;
    closeConnectionCallback m_closecb;
};


#endif // _EPOLLER_H_