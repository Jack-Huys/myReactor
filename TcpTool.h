#ifndef _TCP_TOOL_H_
#define _TCP_TOOL_H_

#include "reactorConmon.h"

// 处理读写的工具        
class TcpTool
{
public:
    TcpTool(int fd);
    ~TcpTool();

public:
    int GetFd() const { return m_fd; }
    int ToolRead();
    int ToolWrite();

private:
    int m_fd;

};














#endif // _TCP_TOOL_H_