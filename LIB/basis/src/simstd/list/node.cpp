#include <basis/simstd/list>

void simstd::pvt::list_node_base::transfer(const list_node_base* first, const list_node_base* last) noexcept
{
	if (first != last)
	{
		auto first1 = m_prev;

		m_prev = last->m_prev;
		m_prev->m_next = this;

		first->m_prev->m_next = const_cast<list_node_base*>(last);
		const_cast<list_node_base*>(last)->m_prev = first->m_prev;

		first1->m_next = const_cast<list_node_base*>(first);
		const_cast<list_node_base*>(first)->m_prev = first1;
	}
}

void simstd::pvt::list_node_base::reverse() noexcept
{
	auto cur = this;
	do {
		auto next = cur->m_next;
		cur->m_next = cur->m_prev;
		cur->m_prev = next;
		cur = cur->m_prev;
	} while (cur != this);
}

void simstd::pvt::list_node_base::hook(const list_node_base* pos) noexcept
{
	m_prev = pos->m_prev;
	m_next = const_cast<list_node_base*>(pos);
	m_prev->m_next = this;
	m_next->m_prev = this;
}

void simstd::pvt::list_node_base::unhook() noexcept
{
	m_prev->m_next = m_next;
	m_next->m_prev = m_prev;
}

void simstd::pvt::list_node_base::swap(list_node_base& a, list_node_base& b) noexcept
{
//	LogTraceLn();
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

//	LogDebug(L"&a: %p\n", &a);
//	LogDebug(L"a.m_next: %p\n", a.m_next);
//	LogDebug(L"a.m_prev: %p\n", a.m_prev);
//	LogDebug(L"&b: %p\n", &b);
//	LogDebug(L"b.m_next: %p\n", b.m_next);
//	LogDebug(L"b.m_prev: %p\n", b.m_prev);
}
