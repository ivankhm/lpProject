#include <iostream>
#include <thread>

#include "Socket.h"

void thread_func() {
	ftp::socket s;

	bool connected = false;
	while (true)
	{
		if (!connected) {
			std::cout << "Client: Trying connect\n";
			connected = s.connect("127.0.0.1", 1035);
		}

		if (connected) {
			std::cout << "Client: Connected\n";
			std::cout << (s.send("hello", 6));
			break;
		}
	}
}

int main(int argc, char * argv[])
{
	ftp::socket s;

	if (s.bind(1035)) {
		std::cout << "Server: Bind\n";
	}
	if (s.listen(5)) {
		std::cout << "Server: Listen\n";
	}

	std::thread thread(thread_func);

	while (true) {
		ftp::socket n(s.accept());

		if (n.is_opened()) {
			char buff[10] = { 0 };
			n.receive(buff, 6);

			if (std::strcmp(buff, "hello") == 0) {
				std::cout << "Server: Equal\n";
				break;
			}
		}
	}
	thread.join();
	std::getchar();
	
	return 0;
}