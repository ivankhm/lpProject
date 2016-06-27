#include "UFAPI.h"
#include <fstream>
#include <iostream>

/*
	Макросы для выделения из строки первого и второго слова
*/
#define FIRSTCUT(x) (std::string(x.begin(), x.begin() + x.find(' ')))
#define LASTCUT(x) (std::string(x.begin() + x.find(' ') + 1, x.end()))


/*
	Конструктор парсит файлы со списком пользователей и списком файлов и записывает данные в объект
*/

DataMap::DataMap()
{
	std::map<std::string, std::string> tmpfiles;
	std::string temporarysting = "";
	std::ifstream usersfile;
	std::ifstream filesfile;

	usersfile.open("users");
	filesfile.open("files");


	while (!filesfile.eof())
	{
		std::getline(filesfile, temporarysting);
		tmpfiles[LASTCUT(temporarysting)] = FIRSTCUT(temporarysting);
	}

	while (!usersfile.eof())
	{
		std::map<std::string, std::string> tmpforusers;
		std::getline(usersfile, temporarysting);
		for (auto &iter : tmpfiles)
		{
			if (iter.first.find(FIRSTCUT(temporarysting)) != std::string::npos)
				tmpforusers[iter.second] = iter.first;
		}
		DataStorage storobj(LASTCUT(temporarysting), tmpforusers);
		data[FIRSTCUT(temporarysting)] = storobj;
	}

	usersfile.close();
	filesfile.close();
}

/*
Деструктор записывает в файл данные о пользователях и файлах из unordered_std::map
Имена у файлов по сравнению с конструктором разные, для дебага, потом поменять
*/
DataMap::~DataMap()
{
	std::ofstream usersfile;
	std::ofstream filesfile;

	usersfile.open("userswrite");
	filesfile.open("fileswrite");

	for (auto &r : data)
	{
		usersfile << r.first << " " << r.second.getPasswd() << std::endl;
		for (auto &x : r.second.getFiles())
		{
			filesfile << x.first << " " << x.second << std::endl;
		}
	}

	usersfile.close();
	filesfile.close();
}

bool DataMap::FindUser(std::string login)
{
	std::unordered_map<std::string, DataStorage>::const_iterator got = data.find(login);
	return got == data.end();
}

void DataMap::RegisterUser(std::string login, std::string passwd)
{
	DataStorage tmp(passwd, std::map<std::string, std::string>());
	data[login] = tmp;
}

void DataMap::GetUserFiles(std::string login)
{
	for (auto &r : data[login].getFiles())
		std::cout << r.first << std::endl;
}

std::string DataMap::GetPathFile(std::string login, std::string filename)
{
	return data[login].getFiles()[filename];
}

void DataMap::debugPrint()
{
	for (auto& x : data) {
		std::cout << x.first << " : " << x.second.getPasswd() << "\n";
		for (auto &r : x.second.getFiles())
			std::cout << "        " << r.first << " : " << r.second << std::endl;
	}
}
