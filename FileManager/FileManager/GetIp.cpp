#include "GetIp.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)

#include "WindowsAPI.h"
#include <stdlib.h>

namespace ftp {
	unsigned long get_ip() {
		static _wsinternal::ws_initializer i;
		char buf[80];
		::gethostname(buf, 80);
		
		hostent * phostent = ::gethostbyname(buf);
		
		if (phostent && phostent->h_addr_list) {
			in_addr addr;
			::memcpy(&addr, phostent->h_addr_list[0], sizeof(struct in_addr));
			return addr.S_un.S_addr;
		}
		return -1;
	}
}
#elif defined(__unix__) || defined(__APPLE__)
#incluede "UnixAPI.h"

unsigned long get_ip() {

	return 0;
}
#endif
