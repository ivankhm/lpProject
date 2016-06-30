#ifndef SERVER_H
#define SERVER_H

#include <list>
#include <mutex>
#include "ControlConnection.h"
#include "UserFilesAPI.h"

namespace ftp {

	class server 
	{
	public:
		typedef socket::port_t port_t;
		typedef std::list<control_connection> connections_t;
		typedef std::mutex mutex_t;

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
		
		std::string create_file(const std::string & login, const std::string & filename) {
			std::lock_guard<mutex_t> lock(mutex_);
			return map_.file_path(login, filename);
		}

		inline data_map::files_t get_list(const std::string &login) {
			return map_.user_files(login);
		}

		inline std::string get_file(const std::string & login, const std::string & file) {
			return map_.file_path(login, file);
		}

		static const std::string DefaultLogin;

	private:
		static bool connection_predicate(const control_connection & src) {
			return !src.is_working();
		}

		inline port_t first_port() {
			return max(FirstDataport, port_);
		}

		control_connection & accept_connection();
		void cleanup_connections();
		port_t get_aviable_port();

		data_map map_;
		mutex_t mutex_;
		port_t port_;
		socket listen_;
		connections_t connections_;

		static const port_t FirstDataport = port_t(1027);
		static const size_t ListenCount = size_t(5);
	};
}

#endif