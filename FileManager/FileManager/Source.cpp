#include "Server.h"
#include "UserFilesAPI.h"

int main(int argc, char * argv[])
{
	ftp::server srv;
	srv.run(1035);

	ftp::data_map map;

	std::getchar();
	return 0;
}
