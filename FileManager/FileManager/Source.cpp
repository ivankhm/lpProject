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
			char buff[20] = { 0 };
			s.receive(buff, 20);

			if (std::strcmp(buff, "Hello world!") == 0) {
				std::cout << "Client: Equal\n" << buff << std::endl;
				break;
			}
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

	std::thread t(thread_func);

	while (true) {
		ftp::socket n(s.accept());

		if (n.is_opened()) {
			n.send("Hello world!", 13);
			break;
		}
	}

	t.join();
	std::getchar();

	return 0;
}