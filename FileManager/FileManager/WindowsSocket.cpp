#include "WindowsSocket.h"

namespace ftp {
	void _wsinternal::init() {
		// При первом вызове функции сконструируется статический объект ->
		// Вызов WSAStartup() ->
		// Последующие вызовы не совершают никакой работы ->
		// По окончанию работы приложения разрушается статический объект ->
		// Вызов WSACleanup
		static _wsinternal::ws_initializer i;
	}

	windows_socket::windows_socket() : raw_(INVALID_SOCKET) {
		_wsinternal::init();
		raw_ = ::socket(PF_INET, SOCK_STREAM, 0);
	}

	windows_socket::windows_socket(SOCKET raw) :
		raw_(raw) { }

	windows_socket::~windows_socket() {
		close();
	}

	void windows_socket::close() {
		if (is_opened()) {
			::shutdown(raw_, SHUT_RDWR);
		}
		::closesocket(raw_);
	}

	bool windows_socket::bind(port_t port = IPPORT_RESERVED) {
		SOCKADDR_IN s_in;
		s_in.sin_family = PF_INET;
		s_in.sin_addr.S_un.S_addr = 0;
		s_in.sin_port = ::htons(port);

		return ::bind(raw_, (LPSOCKADDR)&s_in, sizeof(s_in)) == 0;
	}

	bool windows_socket::listen(size_t count = size_t(1)) {
		return ::listen(raw_, count) == 0;
	}

	bool windows_socket::connect(const char * ptr, port_t port) {
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
		s_in.sin_addr.S_un.S_addr = adres;
		s_in.sin_port = ::htons(port);

		return ::connect(raw_, (LPSOCKADDR)&s_in, sizeof(s_in)) == 0;
	}

	windows_socket * windows_socket::accept() {
		sockaddr_in s_in;

		int length = sizeof(sockaddr);
		SOCKET new_raw = ::accept(raw_, (LPSOCKADDR)&s_in, &length);

		return (new_raw == INVALID_SOCKET) ? nullptr :
			new windows_socket(new_raw);
	}

	size_t windows_socket::send(cdata_t buf, size_t length) {
		const char * ptr = static_cast<const char *>(buf);
		return ::send(raw_, ptr, length, 0);
	}

	size_t windows_socket::receive(data_t buf, size_t length) {
		char * ptr = static_cast<char *>(buf);
		return ::recv(raw_, ptr, length, 0);
	}
}