#include "ControlConnection.h"

namespace ftp {
	void control_connection::action() {
		char buff[100] = { 0 };

		size_t received = socket_.receive(buff, 100);

		std::cout
			<< "Server: Received message\n"
			<< "Count: " << received << "\n"
			<< buff << std::endl;

		socket_.send(buff, received);
	}
}