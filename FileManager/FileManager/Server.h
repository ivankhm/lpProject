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
		
		std::string new_file(std::string &login, std::string &filename){
			std::lock_guard<std::mutex> lock(mutex_);
			return map_.file_path(login, filename);
		}

		inline void get_list(std::string &login){
			map_.user_files(login);
		}

		inline std::string get_filename(std::string &login, std::string &file){
			return map_.file_path(login, file);
		}

		inline std::string get_login(){
			return login;
		}

	private:
		data_map map_;
		mutex_t mutex_;
		std::string login, passw;

		control_connection & accept_connection();
		void cleanup_connections();
		port_t get_aviable_port();

		static bool connection_predicate(const control_connection & src) {
			return !src.is_working();
		}

		port_t port_;
		port_t dataport_;
		socket listen_;
		connections_t connections_;

		static const port_t ReservedDataport = port_t(1027);
		static const size_t ListenCount = size_t(5);
	};
}


#endif