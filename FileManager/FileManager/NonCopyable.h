#ifndef NON_COPYABLE_H
#define NON_COPYABLE_H

namespace ftp {
	namespace utills {
		class non_copyable
		{
		public:
			non_copyable() { }
			~non_copyable() { }

		private:
			non_copyable(const non_copyable & ref);
			non_copyable & operator= (const non_copyable & ref);
		};
	}
}

#endif
