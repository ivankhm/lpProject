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

		server() {
			stop();
		}

		void run(port_t port);
		void stop();

		inline port_t port() const {
			return port_;
		}

		inline bool is_running() const {
			return listen_.is_opened();
		}

	private:
		control_connection & accept_connection();

		port_t port_;
		socket listen_;
		connections_t connections_;
		static const size_t CONNECTION_COUNT = size_t(5);
	};
}


#endif