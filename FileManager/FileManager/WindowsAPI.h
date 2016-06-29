#ifndef WINDOWS_SOCKET_H
#define WINDOWS_SOCKET_H

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
	}

	struct windows_api
	{
		typedef SOCKET socket_t;
		typedef u_short port_t;
		typedef unsigned long addr_t;
		
		static socket_t CreateSocket() {
			static _wsinternal::ws_initializer i;
			return ::socket(PF_INET, SOCK_STREAM, 0);
		}
		static void CloseSocket(socket_t socket) {
			::closesocket(socket);
		}
		static void Shutdown(socket_t socket) {
			::shutdown(socket, 0x02);
		}
		static bool Bind(socket_t socket, port_t port) {
			SOCKADDR_IN s_in;
			s_in.sin_family = PF_INET;
			s_in.sin_addr.S_un.S_addr = 0;
			s_in.sin_port = ::htons(port);

			return ::bind(socket, (LPSOCKADDR)&s_in, sizeof(s_in)) == 0;
		}
		static bool Listen(socket_t socket, size_t count) {
			return ::listen(socket, count) == 0;
		}
		static addr_t ResolveInetAddr(const char * ptr) {
			return ::inet_addr(ptr);
		}
		static addr_t ResolveDNSAddr(const char * ptr) {
			LPHOSTENT p_host = ::gethostbyaddr(ptr, 4, PF_INET);

			if (p_host && p_host->h_addr_list) {
				return ::inet_addr(p_host->h_addr_list[0]);
			}
			delete p_host;

			return InvalidAddress;
		}
		static bool Connect(socket_t socket, addr_t addr, port_t port) {
			SOCKADDR_IN s_in;
			s_in.sin_family = PF_INET;
			s_in.sin_addr.S_un.S_addr = addr;
			s_in.sin_port = ::htons(port);

			return ::connect(socket, (LPSOCKADDR)&s_in, sizeof(s_in)) == 0;
		}
		static socket_t Accept(socket_t socket) {
			SOCKADDR_IN s_in;
			int length = sizeof(sockaddr);

			return ::accept(socket, (LPSOCKADDR)&s_in, &length);
		}
		static size_t Send(socket_t socket, const char * buf, size_t length) {
			return ::send(socket, buf, length, 0);
		}
		static size_t Receive(socket_t socket, char * buf, size_t length) {
			return ::recv(socket, buf, length, 0);
		}

		static const socket_t InvalidSocket = socket_t(-1);
		static const addr_t InvalidAddress = addr_t(-1);
	};

}

#endif