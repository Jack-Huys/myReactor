CC = g++
LD = g++

CFLAGS += -Wall -O2
CFLAGS += -l./inc
CFLAGS += -std=c++11

LDFLAGS += -std=c++11

OBJS += testMian.o TcpTool.o \
		Epoller.o TcpServer.o

testServer : $(OBJS)
	$(LD) $(LDFLAGS) -o testServer $(OBJS)

testMian.o : TcpServer.h

TcpTool.o : TcpTool.h reactorConmon.h

Epoller.o : Epoller.h reactorConmon.h

TcpServer.o : TcpServer.h reactorConmon.h

#伪目标
.PHONY:clear
clear:
	rm -f *.o testServer 
	@echo "clear over!" 