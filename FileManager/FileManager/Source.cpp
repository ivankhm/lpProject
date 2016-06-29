//#include "Server.h"
#include "GetIp.h"
#include <iostream>

int main(int argc, char * argv[])
{
	/*
	ftp::server srv;
	srv.run(1035);
	std::getchar();
	*/
	std::cout << ftp::get_ip() << std::endl;
	
	return 0;
}
