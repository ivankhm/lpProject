#include "Server.h"

namespace ftp {
	void server::run(port_t port) {
		std::cout << "Server: Startup\n";

		listen_.bind(port);
		listen_.listen(CONNECTION_COUNT);

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

	control_connection & server::accept_connection() {
		connections_.emplace_back(listen_.accept(), *this);
		return connections_.back();
	}
}