#include "TcpServer.h"
#include <iostream>

using namespace std;


int main(int argc, char **argv)
{
    int port = 12580;
    if(argc >= 2)
    {
        port = atoi(argv[1]);
    }

    TcpServer server(port);
    server.run();
    return 0;
}  