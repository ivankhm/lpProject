#ifndef CONTROL_CONNECTION_H
#define CONTROL_CONNECTION_H

#include <iostream>
#include "Socket.h"

namespace ftp {
	class server;

	class control_connection 
	{
	public:
		control_connection(socket sock, server & srv) : 
			socket_(std::move(sock)), srv_(srv) { }

		control_connection(control_connection && rhs) : 
			socket_(std::move(rhs.socket_)), srv_(rhs.srv_) { }

		~control_connection() {

		}

		void action();

	private:
		socket socket_;
		server & srv_;
	};
}

#endif