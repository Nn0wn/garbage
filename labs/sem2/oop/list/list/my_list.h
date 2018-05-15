#pragma once
#include <assert.h>
#include <algorithm>
#include <stdexcept>
#include <cstddef>

namespace stepik
{
	template <class Type>
	struct node
	{
		Type value;
		node* next;
		node* prev;

		node(const Type& value, node<Type>* next, node<Type>* prev)
			: value(value), next(next), prev(prev)
		{
		}
	};

	template <class Type>
	class list; //forward declaration

	template <class Type>
	class list_iterator
	{
	public:
		typedef ptrdiff_t difference_type;
		typedef Type value_type;
		typedef Type* pointer;
		typedef Type& reference;
		typedef size_t size_type;
		typedef std::forward_iterator_tag iterator_category;

		list_iterator()
			: m_node(NULL)
		{
		}

		list_iterator(const list_iterator& other)
			: m_node(other.m_node)
		{
		}

		list_iterator& operator = (const list_iterator& other)
		{
			if (this != &other)
			{
				m_node = other.m_node;
			}
			return *this;
		}

		bool operator == (const list_iterator& other) const
		{
			if (m_node == other.m_node)
				return true;
			else
				return false;
		}

		bool operator != (const list_iterator& other) const
		{
			if (m_node != other.m_node)
				return true;
			else
				return false;
		}

		reference operator * ()
		{
			return m_node->value;
		}

		pointer operator -> ()
		{
			return &(m_node->value);
		}

		list_iterator& operator ++ ()
		{
			m_node = m_node->next;
			return *this;
		}

		list_iterator operator ++ (int)
		{
			list_iterator temp = *this;
			++(*this);
			return temp;
		}

	private:
		friend class list<Type>;

		list_iterator(node<Type>* p)
			: m_node(p)
		{
		}

		node<Type>* m_node;
	};

	template <class Type>
	class list
	{
	public:
		typedef Type value_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef list_iterator<Type> iterator;

		list()
			: m_head(nullptr), m_tail(nullptr)
		{
		}

		list::iterator begin()
		{
			return iterator(m_head);
		}

		list::iterator end()
		{
			return iterator();
		}

		~list()
		{
			this->clear();
			delete this->m_head;
			delete this->m_tail;
		}

		list(const list& other)
		{
			m_head = m_tail = nullptr;
			*this = other;
		}

		list(list&& other)
		{
			m_head = m_tail = nullptr;
			*this = other;
			other.clear();
		}

		list& operator= (const list& other)
		{
			this->clear();
			node<Type>* temp = other.m_head;
			while (temp)
			{
				this->push_back(temp->value);
				temp = temp->next;
			}
			return *this;
		}

		void push_back(const value_type& value)
		{
			if (this->empty())
			{
				m_head = new node<Type>(value, nullptr, nullptr);
				m_tail = m_head;
			}
			else
			{
				m_tail->next = new node<Type>(value, nullptr, m_tail);
				m_tail = m_tail->next;
			}
		}

		void push_front(const value_type& value)
		{
			if (this->empty())
			{
				m_head = new node<Type>(value, nullptr, nullptr);
				m_tail = m_head;
			}
			else
			{
				m_head->prev = new node<Type>(value, m_head, nullptr);
				m_head = m_head->prev;
			}
		}

		iterator insert(iterator pos, const Type& value)
		{
			if (pos == end())
			{
				push_back(value);
				return iterator(m_tail);
			}
			if (pos == begin())
			{
				push_front(value);
				return iterator(m_head);
			}
			node<Type>* temp = new node<Type>(value, pos.m_node, pos.m_node->prev);
			temp->next->prev = temp;
			temp->prev->next = temp;
			return iterator(temp);
		}

		iterator erase(iterator pos)
		{
			if (pos == begin())
			{
				pop_front();
				return iterator(m_head);
			}
			if (pos == iterator(m_tail))
			{
				pop_back();
				return iterator();
			}
			node<Type>* temp = pos.m_node;
			iterator ret(temp->next);
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			delete temp;
			return ret;
		}

		reference front()
		{
			return m_head->value;
		}

		const_reference front() const
		{
			return m_head->value;
		}

		reference back()
		{
			return m_tail->value;
		}

		const_reference back() const
		{
			return m_tail->value;
		}

		void pop_front()
		{
			if (m_head)
			{
				if (m_head->next)
				{
					m_head = m_head->next;
					delete m_head->prev;
					m_head->prev = nullptr;
				}
				else
				{
					delete m_head;
					m_head = nullptr;
					m_tail = nullptr;
				}
			}
		}

		void pop_back()
		{
			if (m_tail)
			{
				if (m_tail->prev)
				{
					m_tail = m_tail->prev;
					delete m_tail->next;
					m_tail->next = nullptr;
				}
				else
				{
					delete m_tail;
					m_head = nullptr;
					m_tail = nullptr;
				}
			}
		}

		void clear()
		{
			if(!this->empty())
			{
				node<Type>* temp = m_head;
				while (m_head)
				{
					temp = m_head;
					m_head = m_head->next;
					delete temp;
				}
				m_head = nullptr;
				m_tail = nullptr;
			}		
		}

		bool empty() const
		{
			if (!m_head && !m_tail)
				return true;
			else
				return false;
		}

		size_t size() const
		{
			size_t count = 0;
			node<Type>* temp = m_head;
			while (temp)
			{
				temp = temp->next;
				count++;
			}
			delete temp;
			return count;
		}

		void print()
		{
			node<Type>* temp = m_head;
			while (temp)
			{
				std::cout << temp->value << std::endl;
				temp = temp->next;
			}
			std::cout << "Size = " << this->size() << std::endl;
		}

	private:
		//your private functions

		node<Type>* m_head;
		node<Type>* m_tail;
	};

}// namespace stepik