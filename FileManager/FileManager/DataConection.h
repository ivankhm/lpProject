#ifndef DATA_CONECTION
#define DATA_CONECTION

#include "Socket.h"
#include <fstream>
#include <array>

namespace ftp {

	class data_conection {
	public:
		typedef socket::port_t port_t;
		typedef std::array<char, 1024> buffer_t;

		data_conection(port_t port)
		{
			socket_.bind(port);
		}

		void List() {
			socket_.send("123", 4);
		}
		void send_data(std::string filename){
			std::ifstream fin(filename);
			buffer_t buf;
			while (!fin.eof()) {
				fin.getline(buf.data(), buf.size());
				socket_.send(buf.data(), 1024);
			}
		}
		void receive_data(std::string filename) {
			std::ofstream fout(filename);
			buffer_t buf;
			while (!fout.eof()) {
				socket_.receive(buf.data(), buf.size());
				fout << buf.data();
			}
		}
	private:
		socket socket_;
	};
}
#endif