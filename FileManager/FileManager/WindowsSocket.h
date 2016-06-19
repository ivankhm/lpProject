#ifndef WINDOWS_SOCKET_H
#define WINDOWS_SOCKET_H

#include "BaseSocket.h"

#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")

namespace ftp {
	namespace _wsinternal {
		class ws_initializer
		{
		public:
			ws_initializer() {
				WSADATA data;
				::WSAStartup(MAKEWORD(2, 1), &data);
			}

			~ws_initializer() {
				::WSACleanup();
			}
		};

		void init() {
			// При первом вызове функции сконструируется статический объект ->
			// Вызов WSAStartup() ->
			// Последующие вызовы не совершают никакой работы ->
			// По окончанию работы приложения разрушается статический объект ->
			// Вызов WSACleanup
			static ws_initializer i;
		}
	}

	class windows_socket :
		public base_socket
	{
	public:
		windows_socket() : raw_(INVALID_SOCKET) {
			_wsinternal::init();
			raw_ = ::socket(PF_INET, SOCK_STREAM, 0);
		}

		~windows_socket() {
			close();
		}

		void close() {
			if (is_opened()) {
				::shutdown(raw_, SHUT_RDWR);
			}
			::closesocket(raw_);
		}

		bool bind(port_t port = IPPORT_RESERVED) {
			SOCKADDR_IN s_in;
			s_in.sin_family = PF_INET;
			s_in.sin_addr.S_un.S_addr = 0;
			s_in.sin_port = ::htons(port);

			return ::bind(raw_, (LPSOCKADDR)&s_in, sizeof(s_in)) == 0;
		}

		bool listen(size_t count) {
			return ::listen(raw_, count) == 0;
		}

		bool connect(const char * ptr, port_t port) {
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

		windows_socket * accept() {
			sockaddr_in s_in;

			int length = sizeof(sockaddr);
			SOCKET new_raw = ::accept(raw_, (LPSOCKADDR)&s_in, &length);

			return (new_raw == INVALID_SOCKET) ? nullptr :
				new windows_socket(new_raw);
		}

		size_t send(cdata_t buf, size_t length) {
			const char * ptr = static_cast<const char *>(buf);
			return ::send(raw_, ptr, length, 0);
		}

		size_t receive(data_t buf, size_t length) {
			char * ptr = static_cast<char *>(buf);
			return ::recv(raw_, ptr, length, 0);
		}

		inline bool is_opened() {
			return raw_ != INVALID_SOCKET;
		}

	protected:
		windows_socket(SOCKET raw) : raw_(raw) { }

	private:
		SOCKET raw_;
		const int SHUT_RDWR = 0x02;
	};
}


#endif