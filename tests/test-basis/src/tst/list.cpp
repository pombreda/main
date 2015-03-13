#include <basis/tst.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>

#include <basis/simstd/algorithm>
#include <basis/simstd/memory>
#include <basis/simstd/list>
#include <list>

template<typename Type>
void check_emptiness(const Type& listContainer)
{
	CRT_ASSERT(listContainer.empty());
	CRT_ASSERT(listContainer.size() == 0);
	CRT_ASSERT(simstd::begin(listContainer) == simstd::end(listContainer));
	CRT_ASSERT(simstd::cbegin(listContainer) == simstd::cend(listContainer));
	CRT_ASSERT(listContainer.begin() == listContainer.end());
	CRT_ASSERT(listContainer.cbegin() == listContainer.cend());
	CRT_ASSERT(listContainer.rbegin() == listContainer.rend());
	CRT_ASSERT(listContainer.crbegin() == listContainer.crend());
}

template<typename Type>
void check_size(const Type& listContainer, size_t size)
{
	CRT_ASSERT(!listContainer.empty());
	CRT_ASSERT(listContainer.size() == size);
	CRT_ASSERT(simstd::next(simstd::begin(listContainer), size) == simstd::end(listContainer));
	CRT_ASSERT(simstd::next(simstd::cbegin(listContainer), size) == simstd::cend(listContainer));
	CRT_ASSERT(simstd::next(listContainer.begin(), size) == listContainer.end());
	CRT_ASSERT(simstd::next(listContainer.cbegin(), size) == listContainer.cend());
	CRT_ASSERT(simstd::next(listContainer.rbegin(), size) == listContainer.rend());
	CRT_ASSERT(simstd::next(listContainer.crbegin(), size) == listContainer.crend());
}

ssize_t tst::_list()
{
	LogAtten(L"\n");

	A ma1[5];
	A ma2[4];

	struct HeapTag {};
	using heap_type = memory::heap::DecoratorStat<memory::heap::Special<HeapTag>, memory::heap::StatCount, HeapTag>;
//	using EqAlloc = simstd::allocator<A>;
	using EqAlloc = simstd::AllocatorHeap<A, heap_type>;

	heap_type::init();
	{
		LogTrace();
		using test_list_type = simstd::list<A, EqAlloc>;

		{
			console::printf("\n\n\n");
			LogTrace();
			test_list_type list1;
			check_emptiness(list1);

			EqAlloc allocator;
			test_list_type list2(allocator);
			check_emptiness(list2);

			list1.swap(list2);
			list1.clear();
			check_emptiness(list1);
			list2.clear();
			check_emptiness(list2);
		}

		{
			console::printf("\n\n\n");
			LogTrace();
			test_list_type list1(9, A(9));
			check_size(list1, 9);

			LogTrace();
			EqAlloc allocator;
			test_list_type list2(8, A(8), allocator);
			check_size(list2, 8);

			LogTrace();
			test_list_type list3(list2);
			check_size(list3, 8);

			LogTrace();
			test_list_type list4(list1, allocator);
			check_size(list4, 9);

			LogTrace();
			list4.clear();
			check_emptiness(list4);
			list3.clear();
			check_emptiness(list3);
			list2.clear();
			check_emptiness(list2);
			list1.clear();
			check_emptiness(list1);
		}

		{
			console::printf("\n\n\n");
			LogTrace();
			test_list_type list1(static_cast<test_list_type::size_type>(7));
			check_size(list1, 7);

			LogTrace();
			EqAlloc allocator;
			test_list_type list2(static_cast<test_list_type::size_type>(6), allocator);
			check_size(list2, 6);

			LogTrace();
			test_list_type list3(simstd::move(list2));
			check_size(list3, 6);

			LogTrace();
			test_list_type list4(simstd::move(list1), allocator);
			check_size(list4, 7);

			LogTrace();
			list4.clear();
			check_emptiness(list4);
			list3.clear();
			check_emptiness(list3);
			check_emptiness(list2);
			check_emptiness(list1);
		}

		{
			console::printf("\n\n\n");
			LogTrace();
			test_list_type list1(simstd::begin(ma1), simstd::end(ma1));
			check_size(list1, lengthof(ma1));

			LogTrace();
			EqAlloc allocator;
			test_list_type list2(simstd::begin(ma2), simstd::end(ma2), allocator);
			check_size(list2, lengthof(ma2));

			LogTrace();
			list1.swap(list2);
			check_size(list1, lengthof(ma2));
			check_size(list2, lengthof(ma1));

			LogTrace();
			list2 = list1;
			check_size(list2, list1.size());

			LogTrace();
			list1.clear();
			check_emptiness(list1);
			list2.clear();
			check_emptiness(list2);
		}

//		return 0;
//		test_list_type tl1;
//		test_list_type tl2;
//
//		auto itl1b = tl1.begin();
//		auto itl1e = tl1.end();
//		auto itl2b = tl2.cbegin();
//		auto itl2e = tl2.cend();
//
//		if (itl1b == itl1e)
//			LogTrace();
//		if (itl1b == itl2b)
//			LogTrace();
//		if (itl1b == itl2e)
//			LogTrace();
//		if (itl1e == itl1b)
//			LogTrace();
//		if (itl1e == itl2b)
//			LogTrace();
//		if (itl1e == itl2e)
//			LogTrace();
//		if (itl2b == itl1b)
//			LogTrace();
//		if (itl2b == itl1e)
//			LogTrace();
//		if (itl2b == itl2e)
//			LogTrace();
//		if (itl2e == itl1b)
//			LogTrace();
//		if (itl2e == itl1e)
//			LogTrace();
//		if (itl2e == itl2b)
//			LogTrace();
//
//		A a;
//
//		test_list_type list1;
//		LogReport(L"1:size(): %Iu\n", list1.size());
//		LogReport(L"1:empty(): %d\n", list1.empty());
//
//		list1.emplace_back(1);
//		LogReport(L"1:size(): %Iu\n", list1.size());
//		LogReport(L"1:empty(): %d\n", list1.empty());
//
//		list1.emplace_back(2);
//		LogReport(L"1:size(): %Iu\n", list1.size());
//		LogReport(L"1:empty(): %d\n", list1.empty());
//
//		list1.emplace_back(3);
//		LogReport(L"1:size(): %Iu\n", list1.size());
//		LogReport(L"1:empty(): %d\n", list1.empty());
//
//		list1.reverse();
//
//		list1.pop_back();
//		LogReport(L"1:size(): %Iu\n", list1.size());
//		LogReport(L"1:empty(): %d\n", list1.empty());
//
//		for (auto it = list1.begin(); it != list1.end(); ++it)
//			LogReport(L"%d\n", it->val());
//
//		test_list_type list2(5, a);
//		LogReport(L"2:size(): %Iu\n", list2.size());
//		LogReport(L"2:empty(): %d\n", list2.empty());
//
//		test_list_type list3(10);
//		LogReport(L"3:size(): %Iu\n", list3.size());
//		LogReport(L"3:empty(): %d\n", list3.empty());
//
////		test_list_type list4(simstd::begin(ma), simstd::end(ma));
////		LogReport(L"4:size(): %Iu\n", list4.size());
////		LogReport(L"4:empty(): %d\n", list4.empty());
////
////		test_list_type list4a(list4.begin(), list4.end());
////		LogReport(L"4a:size(): %Iu\n", list4a.size());
////		LogReport(L"4a:empty(): %d\n", list4a.empty());
//
//		test_list_type list5(list2);
//		LogReport(L"5:size(): %Iu\n", list5.size());
//		LogReport(L"5:empty(): %d\n", list5.empty());
//
//		test_list_type list6(simstd::move(list3));
//		LogReport(L"6:size(): %Iu\n", list6.size());
//		LogReport(L"6:empty(): %d\n", list6.empty());
//		LogReport(L"3:size(): %Iu\n", list3.size());
//		LogReport(L"3:empty(): %d\n", list3.empty());
//
//		list1.emplace_back(a);
//
//		list1.push_back(a);
//
//		list1.push_back(a + a);
//
//		list1.push_back(simstd::move(a));
//
////		test_list_type list6;
////		LogReport(L"6:empty(): %d\n", list6.empty());
////		list6.erase(list6.begin());
	}
	heap_type::destroy();

	{
		const auto stat = heap_type::get_stat();
		LogReport(L"Heap '%S' statistics:\n", heap_type::get_name());
		LogReport(L"  alloc: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
		LogReport(L"  free : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
		LogReport(L"  diff : %I64d\n", stat.get_allocations_size() - stat.get_frees_size());
	}
	return 0;
}
