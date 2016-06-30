#include <algorithm>

#include "Server.h"

namespace ftp {

	const std::string server::DefaultLogin = "ftp";

	server::server() : 
		port_(-1) {}

	void server::run(port_t port) {
		std::cout << "Server: Startup\n";

		port_ = port;
		listen_.bind(port_);
		listen_.listen(ListenCount);

		std::cout << "Server: Listening connections\n";
		while (is_running()) 
		{
			accept_connection()
				.start_processing_loop();
			std::cout << "Server: Connection accepted\n";
		}
	}

	void server::stop() {
		listen_.shutdown();
		connections_.clear();
	}

	void server::cleanup_connections() {
		for(connections_t::iterator it = connections_.begin(), end = connections_.end(); it != end; ) 
		{
			if (it->is_working()) { 
				++it; 
			}
			else {
				it = connections_.erase(it);
			}
		}
	}

	server::port_t server::get_aviable_port() {
		port_t port = port_t(-1),
			min_port = port_t(-1),
			max_port = first_port();

		for(const control_connection & x : connections_) 
		{
			if (x.dataport() < min_port) {
				min_port = x.dataport();
			}
			if (x.dataport() > max_port) {
				max_port = x.dataport();
			}
		}

		min_port = min(first_port(), min_port);
		max_port = max(port_t(-1), max_port);

		return min_port + static_cast<port_t>(std::rand() % (max_port - min_port + port_t(1)));
	}

	control_connection & server::accept_connection() {
		cleanup_connections();
		
		connections_.emplace_back(
			listen_.accept(), *this, get_aviable_port()
		);
		return connections_.back();
	}
}