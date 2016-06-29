#ifndef DATA_CONECTION
#define DATA_CONECTION

#include <array>
#include <vector>

#include "Socket.h"

namespace ftp {

	class data_conection 
	{
	public:
		typedef socket::port_t port_t;
		typedef std::array<char, 1024> buffer_t;
		typedef std::vector<std::string> files_t;

		data_conection(port_t port) : port_(port) {
			socket_.bind(port);
		}

		data_conection(const char * ip, port_t port) : port_(port) {
			socket_.connect(ip, port);
		}

		data_conection(data_conection && rhs) : 
			socket_(std::move(rhs.socket_)),
			port_(rhs.port_)
		{ }

		inline bool is_opened() const {
			return socket_.is_opened();
		}

		inline port_t port() const {
			return port_;
		}

		inline void close() {
			socket_.close();
		}

		void reopen();
		void send_files(const files_t & files);
		void send_file(const std::string & filename);
		void save_file(const std::string & filename);
		
	private:
		socket socket_;
		port_t port_;
	};
}
#endif