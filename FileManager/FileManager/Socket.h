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
#include "WindowsAPI.h"
#endif

#ifdef UNIX_OS 
#include "UnixAPI.h"
#endif

namespace ftp {
	
	typedef void * data_t;
	typedef const void * cdata_t;

	namespace _internal {
		template <class TPolicy>
		class socket 
		{
		public:
			typedef typename TPolicy::socket_t socket_t;
			typedef typename TPolicy::port_t port_t;
			typedef typename TPolicy::addr_t addr_t;

			socket() :
				raw_(TPolicy::InvalidSocket) 
			{
				raw_ = TPolicy::CreateSocket();
			}

			~socket() {
				close();
			}

			inline bool is_opened() const {
				return raw_ != TPolicy::InvalidSocket;
			}

			void close() {
				if (is_opened()) {
					shutdown();
				}
				TPolicy::CloseSocket(raw_);
			}

			inline void shutdown() {
				TPolicy::Shutdown(raw_);
			}

			inline bool bind(port_t port) {
				return TPolicy::Bind(raw_, port);
			}

			inline bool listen(size_t count) {
				return TPolicy::Listen(raw_, count);
			}

			bool connect(const char * ptr, port_t port) {
				addr_t addr = TPolicy::ResolveInetAddr(ptr);

				if (addr == TPolicy::InvalidAddress) {
					addr = TPolicy::ResolveDNSAddr(ptr);

					if (addr == TPolicy::InvalidAddress) {
						return false;
					}
				}

				return TPolicy::Connect(raw_, addr, port);
			}

			inline socket accept() const {
				return socket(TPolicy::Accept(raw_));
			}

			size_t send(cdata_t data, size_t length) {
				const char * ptr = static_cast<const char *>(data);
				return TPolicy::Send(raw_, ptr, length);
			}

			size_t receive(data_t data, size_t length) {
				char * ptr = static_cast<char *>(data);
				return TPolicy::Receive(raw_, ptr, length);
			}

		private:
			socket_t raw_;

			socket(socket_t raw) : 
				raw_(raw) { }
		};
	}

#ifdef WINDOWS_OS
	typedef _internal::socket<windows_api> socket;
#endif

#ifdef UNIX_OS 
	typedef _internal::socket<unix_api> socket;
#endif

}

#endif
