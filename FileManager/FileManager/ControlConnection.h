#ifndef CONTROL_CONNECTION_H
#define CONTROL_CONNECTION_H

#include "Socket.h"

class server;

namespace ftp {
	class control_connection 
	{
	public:
		control_connection(socket & sock) :
			socket_(sock.accept()) { }

		void send_hello() {
			if (socket_.is_opened()) {
				socket_.send("Hello world", 11);
			}
		}

	private:
		socket socket_;
	};
}

#endif