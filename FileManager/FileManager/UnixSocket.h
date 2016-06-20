#ifndef UNIX_SOCKET_H
#define UNIX_SOCKET_H

#include <sys/types.h>
#include "BaseSocket.h"

namespace ftp {
	class unix_socket :
		public base_socket
	{
	public:
		unix_socket();
		~unix_socket();

		void close();
		bool bind(port_t port);
		bool listen(size_t count);
		bool connect(const char * addr, port_t port);
		unix_socket * accept();
		size_t send(cdata_t buf, size_t length);
		size_t receive(data_t buf, size_t length);

		inline bool is_opened() {
			return raw_ != INVALID_SOCKET;
		}
	protected:
		unix_socket(int raw);
	private:
		int raw_;
		const int INVALID_SOCKET = -1;
	};
}

#endif