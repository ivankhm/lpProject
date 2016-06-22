#ifndef SERVER_H
#define SERVER_H


#include <vector>
#include "Socket.h"
#include "ControlConnection.h"

namespace ftp {

	class server 
	{
	public:
		typedef std::vector<control_connection> connections_t;

		server() {
			connections_.reserve(10);
		}

		void run (port_t port) {
			socket_.bind(port);
			socket_.listen(CONNECTION_COUNT);
		}

		void stop () {
			socket_.close();
			connections_.clear();
		}

		control_connection & accept_connection() {
			connections_.emplace_back(socket_.accept());
			return connections_.back();
		}

		inline port_t port() const {
			return port_;
		}

		inline bool is_running() const {
			return socket_.is_opened();
		}

	private:
		port_t port_;
		socket socket_;
		connections_t connections_;


		static const int CONNECTION_COUNT = 5;
	};
}


#endif