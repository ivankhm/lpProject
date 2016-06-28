#include <algorithm>

#include "Server.h"

namespace ftp {

	server::server() : 
		port_(-1), 
		dataport_(ReservedDataport)
	{ }

	void server::run(port_t port) {
		std::cout << "Server: Startup\n";

		listen_.bind(port);
		listen_.listen(ListenCount);

		std::cout << "Server: Accepting connections\n";
		while (is_running()) 
		{
			accept_connection()
				.start_processing_loop();
		}
	}

	void server::stop() {
		listen_.shutdown();
		connections_.clear();
	}

	void server::cleanup_connections() {
		for(connections_t::iterator it = connections_.begin(), end = connections_.end(); it != end; ) 
		{
			if (!it->is_working()){
				it = connections_.erase(it);
			} 
			else {
				++it;
			}
		}
	}

	server::port_t server::get_aviable_port() {
		port_t port = port_t(-1);

		for(const control_connection & x : connections_) 
		{
			if (!x.is_working()) {
				port = x.dataport();
				break;
			}
		}

		if (port == port_t(-1))  {
			port = ++dataport_;
			// TODO: Overflow guard!
		}

		return port;
	}


	control_connection & server::accept_connection() {
		port_t port = get_aviable_port();

		cleanup_connections();

		connections_.emplace_back(
			listen_.accept(), *this, port
		);

		return connections_.back();
	}
}