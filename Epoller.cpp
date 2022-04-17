#include "Epoller.h"

Epoller::Epoller()
        :m_epollFd(::epoll_create(MAX_EVENTS)),
        m_events(MAX_EVENTS)
{
       assert(m_epollFd >= 0);
}

Epoller::~Epoller()
{
        ::close(m_epollFd);
}

int Epoller::add(int fd, TcpTool *tool, int events)
{
        struct epoll_event event;
        event.data.ptr = (void*)tool;
        event.events = events;

        int ret = ::epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &event);
        return ret;
}

int Epoller::del(int fd, TcpTool *tool, int events)
{
        struct epoll_event event;
        event.data.ptr = (void *) tool;
        event.events = events;

       int ret = ::epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, &event);
       return ret;
}

int Epoller::mod(int fd, TcpTool *tool, int events)
{  
        struct epoll_event event;
        event.data.ptr = (void *) tool;
        event.events = events;

        int ret = ::epoll_ctl(m_epollFd, EPOLL_CTL_MOD, fd, &event);
        return ret;
}

int Epoller::wait(int timeouts)
{
        int eventsNum = ::epoll_wait(m_epollFd, &*m_events.begin(), static_cast<int>(m_events.size()), timeouts);
    if(eventsNum == 0)
    {

    }else if(eventsNum < 0){

    }

    return eventsNum; 
}

void Epoller::handleEvents(int listenFd, int eventsNum)
{
        assert(eventsNum > 0);
        for(int i = 0; i < eventsNum; ++i)
        {
                TcpTool *tcptool = (TcpTool *)(m_events[i].data.ptr);
                int fd = tcptool->GetFd();
                if(fd == listenFd)
                {
                        m_connentioncb();  // 连接回调
                }else if(m_events[i].events & EPOLLIN)
                {  
                        m_readcb(tcptool); // 可读
            
                }else if(m_events[i].events & EPOLLOUT)
                {  
                        m_writecb(tcptool);  //可写
                }
        
        }
}