#include "TcpServer.h"

int TcpServer::createListenFd(int port)
{
    int _listenFd  = 0;
    _listenFd =  ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if(_listenFd == -1)
    {
        perror("socket erroe");
        return -1;
    }
    struct sockaddr_in  _servAddr; // 通用协议族结构体

    port = ((port <= 1024) || (port >=65536)) ? 12580:port;  // 端口范围
    ::memset(&_servAddr,0,sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_port = ::htons((unsigned short)port);
    _servAddr.sin_addr.s_addr = ::htonl(INADDR_ANY); // 本地IP地址

    int opt = 1;
    // sockfd为需要端口复用的套接字
    setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));

    if((::bind(_listenFd, (struct sockaddr*)&_servAddr, sizeof(_servAddr))) == -1)
    {
        perror("bind fail");
        return -1;
    }

    if((::listen(_listenFd, 10)) == -1) // 设置最大监听队列
    {
        std::cout<<"listen error!"<<std::endl;
        return -1;
    }
    else
    {
        cout << "listening......." << endl;
    }

    return _listenFd;
}

// TcpServer 初始化处理
TcpServer::TcpServer(int port)
            :m_port(port), 
            m_listenFd(createListenFd(port)),
            m_listenTool(new TcpTool(m_listenFd)),
            m_epoll(new Epoller())
{
    assert(m_listenFd >= 0);
}

TcpServer::~TcpServer()
{
}

void TcpServer::acceptConnection()
{
    while (1)
    {
        /* code */
        struct sockaddr_in clientAddr;
        socklen_t len;
        int acceptFd = ::accept(m_listenFd, (struct sockaddr*)&clientAddr, &len);
        if(acceptFd == -1)
        {
            perror("accpet error");
            return;
        }

        TcpTool *tool = new TcpTool(acceptFd);
        m_epoll->add(acceptFd, tool, (EPOLLIN|EPOLLET)); // 接受新连接添加到epoll中
    }
    
}

void TcpServer::closeConnection (TcpTool *tool)
{
    int fd = tool->GetFd();
    m_epoll->del(fd, tool, 0);

    delete tool;
    tool = nullptr;  // 指针置空，防止野指针
}

void TcpServer::handleWrite (TcpTool *tool)
{
    tool->ToolWrite();
}

void TcpServer::handleRead  (TcpTool *tool)
{
    int fd = tool->GetFd();
    int nRead = tool->ToolRead();
    if(nRead == 0 || (nRead <0 && (errno != EAGAIN)))
    {
        closeConnection(tool);
        return ;
    }

    if(nRead < 0 && errno == EAGAIN)
    {
        m_epoll->mod(fd, tool, (EPOLLIN));
        return ; 
    }
}

// 启动服务
void TcpServer::run()
{
    // bind 对回调函数进行绑定
    m_epoll->add(m_listenFd, m_listenTool.get(), (EPOLLIN | EPOLLET));
    m_epoll->setNewConnection(std::bind(&TcpServer::acceptConnection,this));
    m_epoll->setWriteCb(std::bind(&TcpServer::handleWrite,this,std::placeholders::_1));
    m_epoll->setReadCb(std::bind(&TcpServer::handleRead,this,std::placeholders::_1));

//开始事件循环
    while(1){
        int eventsNum = m_epoll->wait(-1);
        if(eventsNum > 0){
            m_epoll->handleEvents(m_listenFd, eventsNum);
        }
    }
}
