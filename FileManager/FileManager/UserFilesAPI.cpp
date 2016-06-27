#include <fstream>
#include <iostream>
#include "UserFilesAPI.h"


namespace ftp {

	const std::string data_map::DataFilename = "_records";
	const std::string data_map::BaseLocation = "Data/";

	data_map::data_map()
	{
		std::string user_line, 
			file_line;

		std::ifstream file(BaseLocation + DataFilename);

		while(!file.eof()) 
		{
			std::getline(file, user_line);

			size_t count = std::stoi(
				cut_number(user_line, 2)
			);

			data_storage::record_t records;

			for(size_t i = 0; i != count; ++i) {
				std::getline(file, file_line);
				records[cut_number(file_line)] = cut_number(file_line, 1);
			}

			data_[cut_number(user_line)] = data_storage(
				cut_number(user_line, 1), std::move(records)
			);
		}
	}

	data_map::~data_map()
	{
	
	}

	data_map::files_t data_map::user_files (const std::string & login)
	{
		files_t files;
		for (auto &r : data_[login].files()) {
			files.push_back(r.first);
		}
		return files;
	}
}