#ifndef _REACTOR_CONMOM_H_
#define _REACTOR_CONMOM_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <sys/epoll.h>
#include <chrono>
#include <cassert>

using namespace std;
#define  MAX_EVENTS  1024  //events
#define  BUFFER_SIZE  1024

#endif // _REACTOR_CONMOM_H_