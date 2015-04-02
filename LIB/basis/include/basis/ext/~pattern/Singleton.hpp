#ifndef BASIS_EXT_PATTERN_SINGLETON_HPP_
#define BASIS_EXT_PATTERN_SINGLETON_HPP_

namespace pattern
{
	template<typename T>
	struct Singleton:
	    private T
	{
		typedef T implementation_type;
		typedef Singleton<T> this_type;
		static T & instance()
		{
			static this_type instance;
			return instance;
		}

	private:
		~Singleton()
		{
		}

		Singleton()
		{
		}
	};
}

#endif
