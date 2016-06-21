#ifndef BASE_SOCKET_H
#define BASE_SOCKET_H

#include "NonCopyable.h"

namespace ftp {

	typedef unsigned short port_t;
	typedef unsigned long addr_t;

	class base_socket :
		public utills::non_copyable
	{
	public:
		typedef void * data_t;
		typedef const void * cdata_t;

		base_socket() { }
		virtual ~base_socket() { }
	};
}

#endif

