#include <basis/sys/memory.hpp>
#include <basis/sys/logger.hpp>
#include <basis/tst.hpp>

#include <basis/simstd/memory>
#include <basis/simstd/string>
#include <string>

ssize_t tst::_string()
{
	TestFuncPlace();

	struct HeapTag {};
	typedef memory::heap::DecoratorStat<memory::heap::Default, memory::heap::StatCount, HeapTag> heap_type;
	typedef simstd::AllocatorHeap<char, heap_type> Allocator;
	typedef simstd::basic_string<char, simstd::char_traits<char>, Allocator> tstring;

	heap_type::init();
	{
		{
			simstd::string qwe1 = "qwerty1";
			simstd::string qwe2 = qwe1;
			qwe2.reserve(qwe2.capacity() + 1);

			TestFuncPlaceFormat("qwe1: size: %Iu, capa: %Iu '%s'\n", qwe1.size(), qwe1.capacity(), qwe1.c_str());
			TestFuncPlaceFormat("qwe2: size: %Iu, capa: %Iu '%s'\n", qwe2.size(), qwe2.capacity(), qwe2.c_str());
			TestFuncPlaceFormat("qwe1.c_str: %p\n", qwe1.c_str());
			TestFuncPlaceFormat("qwe2.c_str: %p\n", qwe2.c_str());

//			qwe2.shrink_to_fit();
			qwe2.pop_back();
//			qwe2.assign(qwe2.c_str(), qwe2.size());
			TestFuncPlaceFormat("qwe1: size: %Iu, capa: %Iu '%s'\n", qwe1.size(), qwe1.capacity(), qwe1.c_str());
			TestFuncPlaceFormat("qwe2: size: %Iu, capa: %Iu '%s'\n", qwe2.size(), qwe2.capacity(), qwe2.c_str());
			TestFuncPlaceFormat("qwe1.c_str: %p\n", qwe1.c_str());
			TestFuncPlaceFormat("qwe2.c_str: %p\n", qwe2.c_str());

			{
				auto qwe3(qwe1 + qwe2);
				TestFuncPlaceFormat("qwe3: size: %Iu, capa: %Iu '%s'\n", qwe3.size(), qwe3.capacity(), qwe3.c_str());
				TestFuncPlaceFormat("qwe3.c_str: %p\n", qwe3.c_str());
				qwe3.erase(3, 4);
				TestFuncPlaceFormat("qwe3: size: %Iu, capa: %Iu '%s'\n", qwe3.size(), qwe3.capacity(), qwe3.c_str());
				TestFuncPlaceFormat("qwe3.c_str: %p\n", qwe3.c_str());
			}
			{
				auto qwe3('a' + qwe1);
				TestFuncPlaceFormat("qwe3: size: %Iu, capa: %Iu '%s'\n", qwe3.size(), qwe3.capacity(), qwe3.c_str());
				TestFuncPlaceFormat("qwe3.c_str: %p\n", qwe3.c_str());
				qwe3.erase(qwe3.cbegin() + 3);
				TestFuncPlaceFormat("qwe3: size: %Iu, capa: %Iu '%s'\n", qwe3.size(), qwe3.capacity(), qwe3.c_str());
				TestFuncPlaceFormat("qwe3.c_str: %p\n", qwe3.c_str());
			}
			{
				auto qwe3("asd" + qwe1);
				TestFuncPlaceFormat("qwe3: size: %Iu, capa: %Iu '%s'\n", qwe3.size(), qwe3.capacity(), qwe3.c_str());
				TestFuncPlaceFormat("qwe3.c_str: %p\n", qwe3.c_str());
				qwe3.erase(qwe3.cbegin() + 3, qwe3.cbegin() + 7);
				TestFuncPlaceFormat("qwe3: size: %Iu, capa: %Iu '%s'\n", qwe3.size(), qwe3.capacity(), qwe3.c_str());
				TestFuncPlaceFormat("qwe3.c_str: %p\n", qwe3.c_str());
			}
			{
				auto qwe3(qwe1 + 'a');
				TestFuncPlaceFormat("qwe3: size: %Iu, capa: %Iu '%s'\n", qwe3.size(), qwe3.capacity(), qwe3.c_str());
				TestFuncPlaceFormat("qwe3.c_str: %p\n", qwe3.c_str());
			}
			{
				auto qwe3(qwe1 + "asd");
				TestFuncPlaceFormat("qwe3: size: %Iu, capa: %Iu '%s'\n", qwe3.size(), qwe3.capacity(), qwe3.c_str());
				TestFuncPlaceFormat("qwe3.c_str: %p\n", qwe3.c_str());
			}
		}

		tstring str1;

		tstring str2(10, 'A');
		TestFuncPlaceFormat("str2: size: %Iu, capa: %Iu '%s'\n", str2.size(), str2.capacity(), str2.c_str());

		tstring str3(str2, 2);
		TestFuncPlaceFormat("str3: size: %Iu, capa: %Iu '%s'\n", str3.size(), str3.capacity(), str3.c_str());

		char str[20] = {'0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9'};

		tstring str4(str, 5);
		TestFuncPlaceFormat("str4: size: %Iu, capa: %Iu '%s'\n", str4.size(), str4.capacity(), str4.c_str());

		tstring str5(str);
		TestFuncPlaceFormat("str5: size: %Iu, capa: %Iu '%s'\n", str5.size(), str5.capacity(), str5.c_str());

		tstring str6(simstd::begin(str), simstd::end(str));
		TestFuncPlaceFormat("str6: size: %Iu, capa: %Iu '%s'\n", str6.size(), str6.capacity(), str6.c_str());

		tstring str7(str6);
		TestFuncPlaceFormat("str7: size: %Iu, capa: %Iu '%s'\n", str7.size(), str7.capacity(), str7.c_str());

		str6.append(6, 'z');
		TestFuncPlaceFormat("str6: size: %Iu, capa: %Iu '%s'\n", str6.size(), str6.capacity(), str6.c_str());

		str6.append(str7);
		TestFuncPlaceFormat("str6: size: %Iu, capa: %Iu '%s'\n", str6.size(), str6.capacity(), str6.c_str());

		str6.append(str7, 3);
		TestFuncPlaceFormat("str6: size: %Iu, capa: %Iu '%s'\n", str6.size(), str6.capacity(), str6.c_str());

		str6.append(1, 'a');
		TestFuncPlaceFormat("str6: size: %Iu, capa: %Iu '%s'\n", str6.size(), str6.capacity(), str6.c_str());
	}
	heap_type::destroy();

	{
		const auto stat = heap_type::get_stat();
		TestFuncPlaceFormat("Heap '%s' statistics:\n", heap_type::get_name());
		TestFuncPlaceFormat("  alloc: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
		TestFuncPlaceFormat("  free : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
		TestFuncPlaceFormat("  diff : %I64d\n", stat.get_allocations_size() - stat.get_frees_size());
	}
	return 0;
}
