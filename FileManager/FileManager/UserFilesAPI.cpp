#include <fstream>
#include <iostream>
#include "UserFilesAPI.h"


namespace ftp {
	data_map::data_map()
	{
	
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