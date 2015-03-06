#include <basis/simstd/list>

namespace simstd
{
	namespace pvt
	{
		void list_node_base::transfer(const list_node_base* first, const list_node_base* last) noexcept
		{
			auto first1 = m_prev;

			m_prev = last->m_prev;
			m_prev->m_next = this;

			first->m_prev->m_next = const_cast<list_node_base*>(last);
			const_cast<list_node_base*>(last)->m_prev = first->m_prev;

			first1->m_next = const_cast<list_node_base*>(first);
			const_cast<list_node_base*>(first)->m_prev = first1;
		}

		void list_node_base::reverse() noexcept
		{
			auto cur = this;
			do {
				auto next = cur->m_next;
				cur->m_next = cur->m_prev;
				cur->m_prev = next;
				cur = cur->m_prev;
			} while (cur != this);
		}

		void list_node_base::hook(const list_node_base* pos) noexcept
		{
//			LogTraceObj();
			auto prev = pos->m_prev;
			auto next = const_cast<list_node_base*>(pos);
//			LogDebug(L"prev: %p\n", prev);
//			LogDebug(L"this: %p\n", this);
//			LogDebug(L"next: %p\n", next);
//			LogDebug(L"prev->m_next: %p\n", prev->m_next);
//			LogDebug(L"prev->m_prev: %p\n", prev->m_prev);
//			LogDebug(L"this->m_next: %p\n", m_next);
//			LogDebug(L"this->m_prev: %p\n", m_prev);
//			LogDebug(L"pos->m_next: %p\n", next->m_next);
//			LogDebug(L"pos->m_prev: %p\n", next->m_prev);

			m_next = const_cast<list_node_base*>(next);
			m_prev = prev;
			prev->m_next = this;
			next->m_prev = this;

//			LogDebug(L"prev->m_next: %p\n", prev->m_next);
//			LogDebug(L"prev->m_prev: %p\n", prev->m_prev);
//			LogDebug(L"this->m_next: %p\n", m_next);
//			LogDebug(L"this->m_prev: %p\n", m_prev);
//			LogDebug(L"pos->m_next: %p\n", next->m_next);
//			LogDebug(L"pos->m_prev: %p\n", next->m_prev);
		}

		void list_node_base::unhook() noexcept
		{
//			LogTraceObj();
			auto prev = m_prev;
			auto next = m_next;
//			LogDebug(L"prev: %p\n", prev);
//			LogDebug(L"this: %p\n", this);
//			LogDebug(L"next: %p\n", next);
//			LogDebug(L"prev->m_next: %p\n", prev->m_next);
//			LogDebug(L"prev->m_prev: %p\n", prev->m_prev);
//			LogDebug(L"this->m_next: %p\n", m_next);
//			LogDebug(L"this->m_prev: %p\n", m_prev);
//			LogDebug(L"next->m_next: %p\n", next->m_next);
//			LogDebug(L"next->m_prev: %p\n", next->m_prev);

			prev->m_next = next;
			next->m_prev = prev;

//			LogDebug(L"prev->m_next: %p\n", prev->m_next);
//			LogDebug(L"prev->m_prev: %p\n", prev->m_prev);
//			LogDebug(L"this->m_next: %p\n", m_next);
//			LogDebug(L"this->m_prev: %p\n", m_prev);
//			LogDebug(L"next->m_next: %p\n", next->m_next);
//			LogDebug(L"next->m_prev: %p\n", next->m_prev);
		}

		void list_node_base::swap(list_node_base& a, list_node_base& b) noexcept
		{
//			LogTrace();
			using simstd::swap;
			swap(a.m_next, b.m_next);
			swap(a.m_prev, b.m_prev);

			if (a.m_next == &b)
				a.m_next = a.m_prev = &a;
			else
				a.m_next->m_prev = a.m_prev->m_next = &a;

			if (b.m_next == &a)
				b.m_next = b.m_prev = &b;
			else
				b.m_next->m_prev = b.m_prev->m_next = &b;

//			LogDebug(L"&a: %p\n", &a);
//			LogDebug(L"a.m_next: %p\n", a.m_next);
//			LogDebug(L"a.m_prev: %p\n", a.m_prev);
//			LogDebug(L"&b: %p\n", &b);
//			LogDebug(L"b.m_next: %p\n", b.m_next);
//			LogDebug(L"b.m_prev: %p\n", b.m_prev);
		}
	}
}
