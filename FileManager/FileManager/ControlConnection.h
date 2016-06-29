#ifndef CONTROL_CONNECTION_H
#define CONTROL_CONNECTION_H

#include <array>
#include <atomic>
#include <iostream>
#include <thread>

#include "DataConnection.h"

namespace ftp {
	class server;
		
	class control_connection 
	{
	public:
		typedef socket::port_t port_t;

		control_connection(socket sock, server & srv, port_t dataport);
		control_connection(control_connection && rhs);
		~control_connection();

		void start_processing_loop();
		void stop_processing_loop();

		inline bool is_working() const {
			return working_.load();
		}

		inline data_conection::port_t dataport() const {
			return datac_.port();
		}
		void send_ip_port();

	private:
		inline std::string cut_number(const std::string src, size_t position = 0) {
			return position == 1 ?
				std::string(src.begin(), src.begin() + src.find(' ')) : position == 2 ?
				std::string(src.begin() + src.find(' ') + 1, src.begin() + src.find_last_of(' ')) :
				std::string(src.begin() + src.find_last_of(' ') + 1, src.end());
		}

		typedef std::array<char, 256> buffer_t;
		typedef std::atomic<bool> bool_t;
		typedef std::thread thread_t;

		void processing_loop();
		void process_command(const buffer_t & cmd, size_t size);
		
		socket socket_;
		thread_t thread_;
		data_conection datac_;
		bool_t working_;
		server & srv_;

		static const size_t MaxEmptyCount = size_t(3);
	};
}

#endif