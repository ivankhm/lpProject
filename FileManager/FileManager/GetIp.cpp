#include "GetIp.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)

#include "WindowsAPI.h"

namespace ftp {
	unsigned long get_ip() {
		static ftp::_wsinternal::ws_initializer i;
		char buf[80];
		gethostname(buf, 80);
		hostent * phe = gethostbyname(buf);
		in_addr addr;
		memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));//без этого никак сорь
		return addr.S_un.S_addr;
	}
}
#elif defined(__unix__) || defined(__APPLE__)

unsigned long get_ip() {
	
	return 0;
}
#endif