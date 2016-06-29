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

	}
		
	void data_conection::send_file(const std::string & filename)
	{
		std::ifstream fin(filename);
		buffer_t buf;
		while (!fin.eof()) {
			fin.getline(buf.data(), buf.size());
			socket_.send(buf.data(), 1024);
		}
	}
		
	void data_conection::save_file(const std::string & filename) 
	{
		std::ofstream fout(filename);
		buffer_t buf;
		while (!fout.eof()) {
			socket_.receive(buf.data(), buf.size());
			fout << buf.data();
		}
	}

	
	
}