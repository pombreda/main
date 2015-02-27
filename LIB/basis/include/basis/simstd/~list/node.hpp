#ifndef BASIS_LIST_NODE_HPP_
#define BASIS_LIST_NODE_HPP_

#include <basis/simstd/list>

namespace simstd {

	namespace pvt {

		struct List_node_base {
			using this_type = List_node_base;

			List_node_base() noexcept;

			void transfer(const this_type* first, const this_type* last) noexcept;

			void reverse() noexcept;

			void hook(const this_type* pos) noexcept;

			void unhook() noexcept;

			static void swap(this_type& a, this_type& b) noexcept;

		public:
			this_type* m_next;
			this_type* m_prev;
		};

		template<typename Type>
		struct List_node: public List_node_base {
			template<typename ... Args>
			List_node(Args &&... args)
				: m_data(simstd::forward<Args>(args)...)
			{
			}

		public:
			Type m_data;
		};

		template<typename Type>
		struct List_iterator {
			using this_type = List_iterator<Type>;
			using node_type = List_node<Type>;

			using iterator_category = simstd::bidirectional_iterator_tag;
			using difference_type = ptrdiff_t;
			using value_type = Type;
			using pointer = Type*;
			using reference = Type&;

			List_iterator() noexcept : pnode() {}

			explicit List_iterator(List_node_base* node) noexcept : pnode(node) {}

//			this_type _M_const_cast() const noexcept {return *this;}

			reference operator *() const noexcept {return static_cast<node_type*>(pnode)->m_data;}

			pointer operator ->() const noexcept {return simstd::addressof(static_cast<node_type*>(pnode)->m_data);}

			this_type& operator ++() noexcept {pnode = pnode->m_next; return *this;}

			this_type operator ++(int) noexcept {this_type tmp = *this; pnode = pnode->m_next; return tmp;}

			this_type& operator --() noexcept {pnode = pnode->m_prev; return *this;}

			this_type operator --(int) noexcept {this_type tmp = *this; pnode = pnode->m_prev; return tmp;}

			bool operator ==(const this_type& other) const noexcept {return pnode == other.pnode;}

			bool operator !=(const this_type& other) const noexcept {return pnode != other.pnode;}

			List_node_base* pnode;
		};

		template<typename Type>
		struct List_const_iterator {
			using this_type = List_const_iterator<Type>;
			using node_type = const List_node<Type>    ;
			using iterator = List_iterator<Type>;

			using iterator_category = simstd::bidirectional_iterator_tag;
			using difference_type = ptrdiff_t;
			using value_type = Type;
			using pointer = const Type*;
			using reference = const Type&;

			List_const_iterator() noexcept : pnode() {}

			explicit List_const_iterator(const List_node_base* node) noexcept : pnode(node) {}

			List_const_iterator(const iterator& it) noexcept : pnode(it.pnode) {}

			iterator iterator_cast() const noexcept {return iterator(const_cast<List_node_base*>(pnode));}

			reference operator *() const noexcept {return static_cast<node_type*>(pnode)->m_data;}

			pointer operator ->() const noexcept {return simstd::addressof(static_cast<node_type*>(pnode)->m_data);}

			this_type& operator ++() noexcept {pnode = pnode->m_next; return *this;}

			this_type operator ++(int) noexcept {this_type tmp = *this; pnode = pnode->m_next; return tmp;}

			this_type& operator --() noexcept {pnode = pnode->m_prev; return *this;}

			this_type operator --(int) noexcept {this_type tmp = *this; pnode = pnode->m_prev; return tmp;}

			bool operator ==(const this_type& other) const noexcept {return pnode == other.pnode;}

			bool operator !=(const this_type& other) const noexcept {return pnode != other.pnode;}

			const List_node_base* pnode;
		};

		template<typename Value>
		bool operator ==(const List_iterator<Value>& a, const List_const_iterator<Value>& b) noexcept
		{
			return a.pnode == b.pnode;
		}

		template<typename Value>
		bool operator !=(const List_iterator<Value>& a, const List_const_iterator<Value>& b) noexcept
		{
			return a.pnode != b.pnode;
		}

	}
}

#endif
