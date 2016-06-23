#include <iostream>
#include "../FileManager/Socket.h"

using std::cout;
using std::cin;

int main(int argc, char * argv[])
{
	ftp::socket s;
	bool connected = false;

	while (!connected) {
		cout << "Client: Trying connect\n";
		connected = s.connect("127.0.0.1", 1035);
	}

	cout << "Client: Connected.\nEnter message:\n";

	char buff[100] = { 0 };

	cin.getline(buff, 100);
	size_t count = s.send(buff, cin.gcount());

	size_t received = s.receive(buff, 100);
	buff[received] = 0;

	cout
		<< "Client: Received message:\n"
		<< buff
		<< "\n";

	std::getchar();
}