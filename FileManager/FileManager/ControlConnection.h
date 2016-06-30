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

		control_connection(socket && sock, server & srv, port_t dataport);
		control_connection(control_connection && rhs);
		~control_connection();

		void start_processing_loop();
		void stop_processing_loop();

		inline bool is_working() const {
			return working_.load();
		}

		inline data_conection::port_t dataport() const {
			return data_.port();
		}

	private:
		typedef std::thread thread_t;
		typedef std::atomic<bool> bool_t;
		typedef std::array<char, 256> buffer_t;
		typedef std::pair<buffer_t, size_t> resp_t;
		typedef resp_t(control_connection::*handler_t)(const buffer_t &);

		void processing_loop();
		void process_command(const buffer_t & cmd, size_t size);

		resp_t invalid_command(const buffer_t & arg);
		resp_t close_control(const buffer_t & arg);
		resp_t open_data(const buffer_t & arg);
		resp_t login(const buffer_t & arg);
		resp_t password(const buffer_t & arg);
		resp_t list(const buffer_t & arg);
		resp_t send_file(const buffer_t & arg);
		resp_t recv_file(const buffer_t & arg);
		resp_t data_access_check(bool &);
		
		socket socket_;
		thread_t thread_;
		data_conection data_;
		bool_t working_;
		server & srv_;
		std::string login_;
		bool authorized_;

		static const size_t MaxEmptyCount = size_t(3);
	};
}

#endif