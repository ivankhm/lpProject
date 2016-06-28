#ifndef SERVER_H
#define SERVER_H

#include <list>

#include "ControlConnection.h"

namespace ftp {

	class server 
	{
	public:
		typedef socket::port_t port_t;
		typedef std::list<control_connection> connections_t;

		server(); 

		~server() {
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
		void cleanup_connections();
		port_t get_aviable_port();

		static bool connection_predicate(const control_connection & src) {
			return !src.is_working();
		}

		port_t port_;
		port_t dataport_;
		socket listen_;
		connections_t connections_;

		static const port_t ReservedDataport = port_t(1027);
		static const size_t ListenCount = size_t(5);
	};
}


#endif