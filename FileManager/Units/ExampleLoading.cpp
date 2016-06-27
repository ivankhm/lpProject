#include <iostream>
#include "UFAPI.h"

int main()
{ 
	/*
		Объект data будет хранить информацию о пользователях и файлах
	*/
	DataMap data;
	data.debugPrint();

	/*
		FindUser - поиск пользователя в таблице
	*/
	std::cout << data.FindUser("user78") << std::endl;
	std::cout << data.FindUser("user1") << std::endl;

	/* 
		Регистрация нового пользователя, можно использовать вместе с FindUser

		data.RegisterUser("newuser", "newpassword");
	*/

	/*
		GetUserFile - получает список файлов для определенного пользователя
	*/
	data.GetUserFiles("user1");

	/*
		GetPathFile - получает полный путь до файла
	*/
	std::cout << data.GetPathFile("user1", "dfile") << std::endl;

	getchar();
	return 0;
}
