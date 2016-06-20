#include "UnixSocket.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

namespace ftp {
	
	unix_socket::unix_socket() : raw_(INVALID_SOCKET) {
		raw_ = ::socket(PF_INET, SOCK_STREAM, 0);
	}
	
	unix_socket::unix_socket(int raw) :
		raw_(raw) { }
	
	unix_socket::~unix_socket() {
		close();
	}
	
	void unix_socket::close() {
		if (is_opened()) {
			::shutdown(raw_, SHUT_RDWR);
		}
		::close(raw_);
	}
	
	bool unix_socket::bind(port_t port = IPPORT_RESERVED) {
		sockaddr_in s_in;
		s_in.sin_family = PF_INET;
		s_in.sin_addr.s_addr = 0;
		s_in.sin_port = htons(port);
		
		return ::bind(raw_, (sockaddr *)&s_in, sizeof(s_in)) == 0;
	}
	
	bool unix_socket::listen(size_t count = size_t(1)) {
		return ::listen(raw_, count) == 0;
	}
	
	bool unix_socket::connect(const char * ptr, port_t port) {
		addr_t adres = ::inet_addr(ptr);
		
		if (adres == -1) {
			hostent * p_host = ::gethostbyaddr(ptr, 4, PF_INET);
			
			if (p_host && p_host->h_addr_list) {
				adres = ::inet_addr(p_host->h_addr_list[0]);
			}
			
			if (adres == -1) {
				return false;
			}
		}
		
		sockaddr_in s_in;
		s_in.sin_family = PF_INET;
		s_in.sin_addr.s_addr = adres;
		s_in.sin_port = htons(port);
		
		return ::connect(raw_, (sockaddr *)&s_in, sizeof(s_in)) == 0;
	}
	
	unix_socket * unix_socket::accept() {
		sockaddr_in s_in;
		
		unsigned int length = sizeof(sockaddr);
		int new_raw = ::accept(raw_, (sockaddr *)&s_in, &length);
		
		return (new_raw == INVALID_SOCKET) ? nullptr :
		new unix_socket(new_raw);
	}
	
	size_t unix_socket::send(cdata_t buf, size_t length) {
		const char * ptr = static_cast<const char *>(buf);
		return ::send(raw_, ptr, length, 0);
	}
	
	size_t unix_socket::receive(data_t buf, size_t length) {
		char * ptr = static_cast<char *>(buf);
		return ::recv(raw_, ptr, length, 0);
	}
	
}