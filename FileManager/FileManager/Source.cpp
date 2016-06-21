#include <iostream>
#include <thread>

#include "Server.h"
#include "ControlConnection.h"

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

			std::cout << buff;
		}
	}
}

int main(int argc, char * argv[])
{
	ftp::server srv;
	
	srv.run(1035);
	std::thread thread(thread_func);

	while (true) {
		srv.accept_connection()
			.send_hello();
	}

	thread.join();
	std::getchar();
	
	return 0;
}