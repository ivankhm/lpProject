#include "ControlConnection.h"
#include "GetIp.h"
#include "Server.h"

namespace ftp {
	control_connection::control_connection(socket sock, server & srv, port_t dataport) : 
		socket_(std::move(sock)), 
		datac_(dataport), 
		working_(false), 
		srv_(srv) 
	{ }

	control_connection::control_connection(control_connection && rhs) : 
		socket_(std::move(rhs.socket_)), 
		thread_(std::move(rhs.thread_)), 
		datac_(std::move(rhs.datac_)), 
		working_(rhs.working_.load()), 
		srv_(rhs.srv_)
	{ }

	control_connection::~control_connection() {
		if (is_working()) {
			stop_processing_loop();
		}
		if (thread_.joinable()) {
			thread_.join();
		}	
	}


	void control_connection::start_processing_loop() {
		working_.store(true);
		thread_ = std::thread { &control_connection::processing_loop, this };
	}

	void control_connection::stop_processing_loop() {
		working_.store(false);
	}

	void control_connection::send_ip_port()
	{
		unsigned long ip = get_ip();
		char buf[6] = {
			(ip >> 24) & 255
			, (ip >> 16) & 255
			, (ip >> 8) & 255
			, (ip) & 255
			, (datac_.port() >> 8)&255
			, datac_.port()&255
		};
		socket_.send(buf, 6);
	}

	void control_connection::processing_loop() {
		buffer_t buffer { };
		size_t empty_count = 0;

		while (is_working()) 
		{
			size_t received = socket_.receive(
				buffer.data(), buffer.size()
			);

			if (received == size_t(0)) {
				++empty_count;
			}

			if ((received == socket::InvalidBytesCount) || 
				(empty_count > MaxEmptyCount)) 
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
		if (std::strcmp(buffer.data(), "PASV") == 0) {
			datac_.reopen();
			// return ip && port!!
			send_ip_port();
		}

		if (std::strcmp(buffer.data(), "LIST") == 0)
		{
			if (datac_.is_opened())
			{
				datac_.send_files(srv_.get_list(srv_.get_login()));
			}
		}

		if (std::strstr(buffer.data(), "RECV") == buffer.data()) {
			srv_.new_file(srv_.get_login(), cut_number(buffer.data(), 1));
			datac_.save_file(cut_number(buffer.data(), 1));
		}

		if (std::strstr(buffer.data(), "STOR") == buffer.data()) {
			srv_.get_filename(srv_.get_login(), cut_number(buffer.data(), 1));
			datac_.send_file(cut_number(buffer.data(), 1));
		}

		socket_.send(buffer.data(), size);
	}
}