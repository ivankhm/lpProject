#include "Server.h"

int main(int argc, char * argv[])
{
	ftp::server srv;
	srv.run(1026);
	std::getchar();

	return 0;
}
