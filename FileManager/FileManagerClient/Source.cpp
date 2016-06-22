#include <iostream>
#include "../FileManager/Socket.h"

int main(int argc, char * argv[])
{
	ftp::socket s;

	bool connected = false;
	while (!connected) {
		std::cout << "Client: Trying connect\n";
		connected = s.connect("127.0.0.1", 1035);
	}

	std::cout << "Client: Connected.\n";
	size_t count = s.send("Hello", 6);
	std::cout << count << "\n" << WSAGetLastError();

	char buff[100] = { 0 };
	s.receive(buff, 100);

	std::cout << buff;
	std::getchar();
}