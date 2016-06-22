#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include "ControlConnection.h"

namespace ftp {

	class server 
	{
	public:
		typedef socket::port_t port_t;
		typedef std::vector<control_connection> connections_t;

		server() { }

		void run (port_t port) {
			std::cout << "Server: Startup\n";

			socket_.bind(port);
			socket_.listen(CONNECTION_COUNT);

			std::cout << "Server: Acceptiong connections\n";
			while (is_running()) {
				control_connection & conn = accept_connection();

				std::cout << "Server: Client connected\n";
				conn.action();

				break;
			}
		}

		void stop () {
			socket_.shutdown();
			connections_.clear();
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

		control_connection & accept_connection() {
			connections_.push_back(
				control_connection(socket_.accept(), *this)
			);
			return connections_.back();
		}

		static const int CONNECTION_COUNT = 5;
	};
}


#endif