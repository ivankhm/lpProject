#ifndef SOCKET_H
#define SOCKET_H

#if defined(_WIN32) || defined(WIN32) || \
	defined(_WIN64) || defined(WIN64)
#define WINDOWS_OS
#elif defined(__unix__) || defined(__APPLE__)
#define UNIX_OS
#endif

#include "NonCopyable.h"
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
		class socket :
			public utills::non_copyable
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

			socket(socket && rhs) : 
				raw_(rhs.raw_) 
			{
				rhs.raw_ = TSystem::InvalidSocket;
			}

			~socket() {
				close();
			}

			socket & operator= (socket && rhs) 
			{
				if (this != &rhs) 
				{
					raw_ = rhs.raw_;
					rhs.raw_ = TSystem::InvalidSocket;
				}
				return *this;
			}

			inline bool is_opened() const {
				return raw_ != TSystem::InvalidSocket;
			}

			void close() {
				if (is_opened()) {
					shutdown();
					TSystem::CloseSocket(raw_);
				}
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

				if (addr == TSystem::InvalidAdres) {
					addr = TSystem::ResolveDNSAddr(ptr);

					if (addr == TSystem::InvalidAdres) {
						return false;
					}
				}

				return TSystem::Connect(raw_, addr, port);
			}

			inline socket accept() const {
				return socket(TSystem::Accept(raw_));
			}

			inline size_t send(cdata_t data, size_t length) const {
				const char * ptr = static_cast<const char *>(data);
				return TSystem::Send(raw_, ptr, length);
			}

			inline size_t receive(data_t data, size_t length) const {
				char * ptr = static_cast<char *>(data);
				return TSystem::Receive(raw_, ptr, length);
			}

			static const size_t InvalidBytesCount = size_t(-1);

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

	namespace _internal {
		template<class TSystem>
		struct utill {
			static inline ftp::socket::addr_t MachineIp() {
				return TSystem::MachineIp();
			}
		};
	}

#ifdef WINDOWS_OS
	typedef _internal::utill<windows_api> utill;
#endif

#ifdef UNIX_OS 
	typedef _internal::utill<unix_api> utill;
#endif

}
#endif
