#ifndef SOCKET_H
#define SOCKET_H

#if defined(_WIN32) || defined(WIN32) || \
	defined(_WIN64) || defined(WIN64)
#define WINDOWS_OS
#elif defined(__unix__) || defined(__APPLE__)
#define UNIX_OS
#endif

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
		template <class TSystem>
		class socket 
		{
		public:
			typedef typename TSystem::socket_t socket_t;
			typedef typename TSystem::port_t port_t;
			typedef typename TSystem::addr_t addr_t;

			socket() :
				raw_(TSystem::InvalidSocket) 
			{
				raw_ = TSystem::CreateSocket();
			}

			~socket() {
				close();
			}

			inline bool is_opened() const {
				return raw_ != TSystem::InvalidSocket;
			}

			void close() {
				if (is_opened()) {
					shutdown();
				}
				TSystem::CloseSocket(raw_);
			}

			inline void shutdown() {
				TSystem::Shutdown(raw_);
			}

			inline bool bind(port_t port) {
				return TSystem::Bind(raw_, port);
			}

			inline bool listen(size_t count) {
				return TSystem::Listen(raw_, count);
			}

			bool connect(const char * ptr, port_t port) {
				addr_t addr = TSystem::ResolveInetAddr(ptr);

				if (addr == TSystem::InvalidAddress) {
					addr = TSystem::ResolveDNSAddr(ptr);

					if (addr == TSystem::InvalidAddress) {
						return false;
					}
				}

				return TSystem::Connect(raw_, addr, port);
			}

			inline socket accept() {
				return socket(TSystem::Accept(raw_));
			}

			size_t send(cdata_t data, size_t length) {
				const char * ptr = static_cast<const char *>(data);
				return TSystem::Send(raw_, ptr, length);
			}

			size_t receive(data_t data, size_t length) {
				char * ptr = static_cast<char *>(data);
				return TSystem::Receive(raw_, ptr, length);
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
