#ifndef UF_H
#define UF_H

#include <unordered_map>
#include <vector>
#include <string>

class data_storage
{
public:
	typedef std::unordered_map<std::string, std::string> record_t;

	data_storage(const std::string & password) : 
		password_(password), files_() { }

	data_storage() { }

	~data_storage() { }

	inline std::string & passwordd() { 
		return password_; 
	}

	inline record_t & files() { 
		return files_; 
	}

private:

	inline static std::string first_cut (const std::string & x) {
		return std::string(x.begin(), x.begin() + x.find(' '));
	}

	inline static std::string last_cut (const std::string & x) {
		return std::string(x.begin() + x.find(' ') + 1, x.end());
	}

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
		data_[login] = data_storage(password);
	}

private:
	data_t data_;
};

#endif
