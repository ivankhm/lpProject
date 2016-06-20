#ifndef BASE_SOCKET_H
#define BASE_SOCKET_H

#include "NonCopyable.h"

namespace ftp {

	class base_socket :
		public utills::non_copyable
	{
	public:
		typedef unsigned short port_t;
		typedef unsigned long addr_t;
		typedef void * data_t;
		typedef const void * cdata_t;

		base_socket() { }
		virtual ~base_socket() { }
	};
}

#endif

