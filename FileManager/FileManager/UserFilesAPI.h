#ifndef UF_H
#define UF_H

#include <unordered_map>
#include <vector>
#include <string>

namespace ftp {
	class data_storage
	{
	public:
		typedef std::unordered_map<std::string, std::string> record_t;

		data_storage() { }

		data_storage(const std::string & password) : 
			password_(password) { }

		data_storage(const std::string & password, record_t && files) : 
			password_(password), files_(std::move(files)) { }

		~data_storage() { }

		inline std::string & password() { 
			return password_; 
		}

		inline record_t & files() { 
			return files_; 
		}

	private:

		std::string password_;
		record_t files_;
	};

	class data_map 
	{
	public:
		typedef std::unordered_map<std::string, data_storage> data_t;
		typedef std::vector<std::string> files_t;

		data_map();
		~data_map();

		files_t user_files (const std::string & login);

		inline std::string file_path (const std::string & login, const std::string & file) {
			return data_[login].files()[file];
		}

		inline bool contains_user (const std::string & login) {
			return data_.find(login) == data_.end();
		}

		inline void register_user (const std::string & login, const std::string & password) {
			data_[login] = data_storage(password, data_storage::record_t()); 
		}

	private:

		inline std::string cut_number(const std::string src, size_t position = 0) {
			return position == 1 ?
				std::string(src.begin(), src.begin() + src.find(' ')) : position == 2 ?
				std::string(src.begin() + src.find(' ') + 1, src.begin() + src.find_last_of(' ')) :
				std::string(src.begin() + src.find_last_of(' ') + 1, src.end());
		}

		data_t data_;

		static const std::string DataFilename;
		static const std::string BaseLocation;
	};
}

#endif
