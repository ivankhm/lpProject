#ifndef WINDOWS_SOCKET_H
#define WINDOWS_SOCKET_H

#include "BaseSocket.h"

#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")

namespace ftp {
	namespace _wsinternal {
		void init();

		class ws_initializer
		{
		public:
			ws_initializer() {
				WSADATA data;
				::WSAStartup(MAKEWORD(1, 1), &data);
			}
			~ws_initializer() {
				::WSACleanup();
			}
		};
	}

	class windows_socket :
		public base_socket
	{
	public:
		windows_socket();
		~windows_socket();

		void close();
		bool bind(port_t port);
		bool listen(size_t count);
		bool connect(const char * addr, port_t port);
		windows_socket * accept();
		size_t send(cdata_t buf, size_t length);
		size_t receive(data_t buf, size_t length);

		inline bool is_opened() {
			return raw_ != INVALID_SOCKET;
		}
	protected:
		windows_socket(SOCKET raw);
	private:
		SOCKET raw_;
		const int SHUT_RDWR = 0x02;
	};
}


#endif