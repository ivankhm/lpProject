#ifndef SOCKET_H
#define SOCKET_H

#if defined(_WIN32) || defined(WIN32) || \
	defined(_WIN64) || defined(WIN64)
#define WINDOWS_OS
#elif defined(__unix__) || defined(__APPLE__)
#define UNIX_OS
#endif

#include "BaseSocket.h"
#ifdef WINDOWS_OS
#include "WindowsSocket.h"
#endif

#ifdef UNIX_OS 
#include "UnixSocket.h"
#endif

namespace ftp {
	namespace _internal {
		template <class TSocket>
		class socket :
			public base_socket
		{
		public:
			socket() :
				impl_(new TSocket()) { }

			socket(base_socket * impl) :
				impl_(static_cast<TSocket*>(impl)) { }

			~socket() {
				close();
				delete impl_;
			}

			inline bool is_opened() {
				return impl_ != nullptr && impl_->is_opened();
			}

			inline void close() {
				if (impl_ != nullptr) {
					impl_->close();
				}
			}

			inline bool bind(port_t port) {
				return impl_->bind(port);
			}

			inline bool listen(size_t count) {
				return impl_->listen(count);
			}

			inline bool connect(const char * addr, port_t port) {
				return impl_->connect(addr, port);
			}

			inline base_socket * accept() {
				return impl_->accept();
			}

			inline size_t send(cdata_t data, size_t length) {
				return impl_->send(data, length);
			}

			inline size_t receive(data_t data, size_t length) {
				return impl_->receive(data, length);
			}

		private:
			TSocket * impl_;
		};
	}

#ifdef WINDOWS_OS
	typedef _internal::socket<windows_socket> socket;
#endif

#ifdef UNIX_OS 
	typedef _internal::socket<unix_socket> socket;
#endif

}

#endif
