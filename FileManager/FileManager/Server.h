#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <array>
#include "ControlConnection.h"

namespace ftp {

	class server 
	{
		static const size_t CONNECTION_COUNT = size_t(5);
	public:
		typedef socket::port_t port_t;
		typedef std::vector<control_connection> connections_t;
		typedef std::array<bool, CONNECTION_COUNT> ports_t;
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
		
		inline port_t get_avalible_ports() {
			size_t i(0);
			while (avalible_ports[i]) ++i;
			return 1025 + i;
		}
		
		inline void take_port(port_t t) {
			avalible_ports[t - 1025] = true;
		}

		inline void free_port(port_t t) {
			avalible_ports[t - 1025] = false;
		}

	private:
		control_connection & accept_connection();

		port_t port_;
		ports_t avalible_ports;
		socket listen_;
		connections_t connections_;
		
		
	};
}


#endif