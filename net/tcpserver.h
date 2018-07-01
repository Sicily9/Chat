/*
 * tcpserver.h
 *
 *  Created on: 2018年3月21日
 *      Author: xc
 */

#ifndef NET_TCPSERVER_H_
#define NET_TCPSERVER_H_
#include "tcpconnection.h"
#include <atomic>
#include <map>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace xc{
	namespace net{
		class Acceptor;
		class EventLoop;
		class EventLoopThreadPool;

		class TcpServer: boost::noncopyable{
		public:
		typedef boost::function<void(EventLoop*)> ThreadInitCallback;
		enum Option
		{
		kNoReusePort,
		kReusePort,
		};

		TcpServer(EventLoop* loop, const InetAddress& listenAddr,
				const std::string& nameArg, Option option = kNoReusePort);
		~TcpServer();

		const std::string& hostport() const { return _hostport; }
		const std::string& name() const { return _name; }
		EventLoop* getLoop() const { return _loop; }

		void setThreadNum(int numThreads);
		void setThreadInitCallback(const ThreadInitCallback& cb){ _threadInitCallback = cb; }

		EventLoopThreadPool* threadPool() { return get_pointer(_threadPool); }

		void start();

		void setConnectionCallback(const ConnectionCallback& cb){  _connectionCallback = cb; }

		void setMessageCallback(const MessageCallback& cb){ _messageCallback = cb; }

		void setWriteCompleteCallback(const WriteCompleteCallback& cb){ _writeCompleteCallback = cb; }

		private:

		void newConnection(int sockfd, const InetAddress& peerAddr);

		void removeConnection(const TcpConnectionPtr& conn);

		void removeConnectionInLoop(const TcpConnectionPtr& conn);

		typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

		EventLoop* _loop;
		const std::string _hostport;
		const std::string _name;
		boost::scoped_ptr<Acceptor> _acceptor;
		boost::scoped_ptr<EventLoopThreadPool> _threadPool;
		ConnectionCallback _connectionCallback;
		MessageCallback _messageCallback;
		WriteCompleteCallback _writeCompleteCallback;
		ThreadInitCallback _threadInitCallback;
		std::atomic_int _started;
		int _nextConnId;
		ConnectionMap _connections;
		};
	}
}




#endif /* NET_TCPSERVER_H_ */
