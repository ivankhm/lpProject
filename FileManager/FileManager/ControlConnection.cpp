#include "ControlConnection.h"

namespace ftp {
	void control_connection::start_processing_loop() {
		working_.store(true);
		thread_ = std::thread { &control_connection::processing_loop, this };
	}

	void control_connection::stop_processing_loop() {
		working_.store(false);
	}

	void control_connection::processing_loop() {
		buffer_t buffer { };
		size_t null_count = 0;

		while (is_working()) 
		{
			size_t received = socket_.receive(
				buffer.data(), buffer.size()
			);

			if (received == size_t(0)) {
				++null_count;
			}

			if (received == socket::InvalidBytesCount || 
				null_count > MaxEmptyCount) 
			{
				stop_processing_loop();
			}
			else 
			{
				process_command(buffer, received);
			}
		}
	}

	void control_connection::process_command(const buffer_t & buffer, size_t size) {
		if (std::strcmp(buffer.data(), "QUIT") == 0) {
			stop_processing_loop();
		}

		socket_.send(buffer.data(), size);
	}
}