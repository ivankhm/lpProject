#ifndef SERVER_H
#define SERVER_H


#include <vector>
#include "Socket.h"
#include "ControlConnection.h"

namespace ftp {

	class server 
	{
	public:
		server() : is_running_(false) { }

		void run (port_t port) {
			if(socket_.bind(port)) {
				is_running_ = socket_.listen(CONNECTION_COUNT);
			}
		}

		void stop () {
			socket_.close();
			is_running_ = false;
		}

		control_connection & accept_connection() {
			connections_.emplace_back(socket_);
			return connections_.back();
		}

		inline port_t port() const {
			return port_;
		}

		inline bool is_running() const {
			return is_running_;
		}

	private:
		socket socket_;
		port_t port_;
		bool is_running_;
		std::vector<control_connection> connections_;


		static const int CONNECTION_COUNT = 5;
	};
}


#endif