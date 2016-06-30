#include "DataConnection.h"
#include <fstream>

namespace ftp {

	void data_conection::reopen() {
		if (!socket_.is_opened())
		{
			socket_ = socket();
			socket_.bind(port_);
		}
	}

	void data_conection::send_files(const files_t & files) {
		for (const std::string &i : files) {
			socket_.send(i.data(), i.length());
		}
	}
		
	void data_conection::send_file(const std::string & filename)
	{
		buffer_t buffer;
		std::ifstream fin(filename);

		while (!fin.eof()) 
		{
			fin.getline(buffer.data(), buffer.size());
			socket_.send(buffer.data(), fin.gcount());
		}
	}
		
	void data_conection::recv_file(const std::string & filename) 
	{
		buffer_t buffer;
		std::ofstream fout(filename);

		while (!fout.eof()) 
		{
			size_t received = socket_.receive(buffer.data(), buffer.size());
			
			if (received > 0) {
				fout.write(buffer.data(), received);
			}
		}
	}
}