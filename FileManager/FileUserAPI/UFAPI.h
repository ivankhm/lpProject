#ifndef UF_H
#define UF_H

#include <unordered_map>
#include <string>
#include <map>

class DataStorage{
private:
	std::string password;
	std::map<std::string, std::string> files;
public:
	DataStorage(std::string _password, std::map<std::string, std::string> _files) : password(_password), files(_files) { };
	DataStorage() { };
	~DataStorage() { };

	std::string getPasswd() { return password; };
	std::map<std::string, std::string> getFiles() { return files; };
};

class DataMap {
private:
	std::unordered_map<std::string, DataStorage> data;
public:
	DataMap();
	~DataMap();

	bool FindUser(std::string);
	void RegisterUser(std::string, std::string);
	void GetUserFiles(std::string);
	std::string GetPathFile(std::string, std::string);

	void debugPrint();
};

#endif
