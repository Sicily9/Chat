BIN=main

CC=g++

LIBS= -lrt  -lpthread -lprotobuf -lmysqlclient

CXXFLAGS= -std=c++11

all:$(BIN)
	
main:chat_main.cpp ../net/reactor.h ../net/reactor.cpp  \
../net/handler.h ../net/handler.cpp ../net/poller/basePoller.h \
../net/poller/basePollerFactory.h ../net/poller/basePollerFactory.cpp \
../net/poller/epollPoller.h ../net/poller/epollPoller.cpp  \
../utility/log/log.h ../utility/log/log_impl.h ../utility/log/log_impl.cpp \
../utility/ringbuffer/ringbuffer.h ../net/acceptor.h ../net/acceptor.cpp \
../net/socket.h ../net/socket.cpp ../net/socketop.h ../net/socketop.cpp \
../net/inetaddr.h ../net/inetaddr.cpp ../utility/buffer/buffer.cpp \
../utility/dispatcher/dispatcher.cpp ../utility/dispatcher/dispatcher.h \
../utility/dispatcher/transportData.h ../utility/msghandler/msghandler.cpp \
../utility/msghandler/msghandler.h ../utility/buffer/buffer.h ../net/tcpserver.h ../net/tcpserver.cpp \
../net/tcpconnection.h ../net/tcpconnection.cpp ../net/reactorthread.h \
../net/reactorthread.cpp ../net/reactorthreadpool.h ../net/reactorthreadpool.cpp \
include/chatServer/chatServer.h chatServer/chatServer.cpp include/db/db.h \
db/db.cpp include/proto/TcpProto.pb.h proto/TcpProto.pb.cc \
include/room/room.h room/room.cpp include/user/user.h include/codec/codec.h \
include/room/room_internal.h room/room_internal.cpp
	$(CC) -o $@ $^ $(LIBS) $(CXXFLAGS)
	@echo 编译成功!	

clean:
	rm -f  $(BIN)

