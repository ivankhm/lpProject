#ifndef CONTROL_CONNECTION_H
#define CONTROL_CONNECTION_H

#include <array>
#include <atomic>
#include <iostream>
#include <thread>
#include "Socket.h"

namespace ftp {
	class server;

	class control_connection 
	{
	public:
		control_connection(socket sock, server & srv) : 
			socket_(std::move(sock)), thread_(), working_(false), srv_(srv) { }

		control_connection(control_connection && rhs) : 
			socket_(std::move(rhs.socket_)), thread_(std::move(rhs.thread_)), 
			working_(rhs.working_.load()), srv_(rhs.srv_) { }

		~control_connection() 
		{
			if (is_working()) {
				stop_processing_loop();
			}
			if (thread_.joinable()) {
				thread_.join();
			}
		}

		void start_processing_loop();
		void stop_processing_loop();

		inline bool is_working() const {
			return working_.load();
		}

	private:
		typedef std::array<char, 256> buffer_t;
		typedef std::atomic<bool> bool_t;
		typedef std::thread thread_t;

		void processing_loop();
		void process_command(const buffer_t & cmd, size_t size);

		socket socket_;
		thread_t thread_;
		bool_t working_;
		server & srv_;

		static const size_t MaxEmptyCount = size_t(3);
	};
}

#endif