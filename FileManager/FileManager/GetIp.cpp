#include "GetIp.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)

#include "WindowsAPI.h"
#include <stdlib.h>

namespace ftp {
	unsigned long get_ip() {
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
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

unsigned long get_ip() {
	struct ifaddrs * ifAddrStruct = NULL;
	unsigned long ip;
	getifaddrs(&ifAddrStruct);
	if (!ifAddrStruct) return -1;
	ip = ((struct sockaddr_in *)ifAddrStruct[9].ifa_addr)->sin_addr.s_addr;//под индексом 9 лежит наш ip адрес. хз почему именно так, но оно работает.
	freeifaddrs(ifAddrStruct);
	return ip;
}
#endif
