#include "TcpTool.h"

TcpTool::TcpTool(int fd)
            :m_fd(fd)
{
    assert(m_fd >= 0);
}

TcpTool::~TcpTool()
{
    close(m_fd);
}

int TcpTool::ToolRead()
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    int ret = recv(m_fd, buffer, sizeof(buffer), 0);

    if(ret == 0)
    { 
        //断开连接
        std::cout<<"link error!"<<std::endl;
    }else if(ret > 0)
    {
         cout<<"recv from clit:"<<buffer<<endl;
    }else if(ret == -1){
        cout<<"recv error!"<<endl;
        cout<<strerror(errno)<<endl;
    }

    return ret;

}

int TcpTool::ToolWrite()
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    const char * str = "This is csl sever!";
    memcpy(buffer,str,sizeof(str));
    int ret = send(m_fd, buffer, sizeof(buffer), 0);
    if(ret > 0)
    {
        cout<<"send success!"<<endl;
    }else if(ret == -1)
    {
        cout<<"send error!"<<endl;
    }

    return ret;
}
