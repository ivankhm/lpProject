#ifndef CONTROL_CONNECTION_H
#define CONTROL_CONNECTION_H

#include "Socket.h"

namespace ftp {
	class server;

	class control_connection 
	{
	public:
		control_connection(const socket & sock, server & srv) : 
			socket_(sock), srv_(srv) { }

		~control_connection() {

		}

		void action() {
			char buff[100] = { 0 };

			size_t received = socket_.receive(buff, 100);

			std::cout 
				<< "Server: Received message\n" 
				<< "Count: " << received << "\n"
				<< "Error: " << WSAGetLastError() << "\n"
				<< buff << std::endl;

			socket_.send(buff, received);
		}

	private:
		socket socket_;
		server & srv_;
	};
}

#endif