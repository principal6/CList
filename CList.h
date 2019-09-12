#pragma once

#include <iostream>
#include <initializer_list>
#include <cassert>

#define SAFE_DELETE(o) if (o) { delete o; o = nullptr; }

using DataType = int;

class CList
{
	struct SListNode
	{
		SListNode(DataType _Data) : Data{ _Data } {}

		SListNode* Prev{};
		SListNode* Next{};
		DataType Data{};
	};

	using node = SListNode;

	class CListBaseIterator
	{
	public:
		CListBaseIterator(node* Node, node* Back) : PtrNode{ Node }, PtrRecovery{ Back } {}
		virtual ~CListBaseIterator() {}

		virtual CListBaseIterator& operator++() noexcept = 0;
		virtual CListBaseIterator& operator--() noexcept = 0;

		bool operator==(const CListBaseIterator& b) const noexcept
		{
			return (PtrNode == b.PtrNode) ? true : false;
		}

		bool operator!=(const CListBaseIterator& b) const noexcept
		{
			return (PtrNode != b.PtrNode) ? true : false;
		}

		node* PtrNode{};
		node* PtrRecovery{};
	};

	class CListIterator final : public CListBaseIterator
	{
	public:
		CListIterator(node* Node, node* Back) : CListBaseIterator(Node, Back) {}
		virtual ~CListIterator() {}

		CListIterator& operator++() noexcept override
		{
			assert(PtrNode);
			PtrNode = PtrNode->Next;
			return *this;
		}

		CListIterator& operator--() noexcept override
		{
			if (PtrNode)
			{
				PtrNode = PtrNode->Prev;
			}
			else
			{
				PtrNode = PtrRecovery;
			}
			return *this;
		}

		DataType& operator*() const
		{
			assert(PtrNode);
			return PtrNode->Data;
		}
	};

	class CListReverseIterator final : public CListBaseIterator
	{
	public:
		CListReverseIterator(node* Node, node* Back) : CListBaseIterator(Node, Back) {}
		virtual ~CListReverseIterator() {}

		CListReverseIterator& operator++() noexcept override
		{
			assert(PtrNode);
			PtrNode = PtrNode->Prev;
			return *this;
		}

		CListReverseIterator& operator--() noexcept override
		{
			if (PtrNode)
			{
				PtrNode = PtrNode->Next;
			}
			else
			{
				PtrNode = PtrRecovery;
			}
			return *this;
		}

		DataType& operator*() const
		{
			assert(PtrNode);
			return PtrNode->Data;
		}
	};

	class CListConstIterator final : public CListBaseIterator
	{
	public:
		CListConstIterator(node* Node, node* Back) : CListBaseIterator(Node, Back) {}
		CListConstIterator(const CListIterator& Iter) : CListBaseIterator(Iter.PtrNode, Iter.PtrRecovery) {}
		virtual ~CListConstIterator() {}

		CListConstIterator& operator++() noexcept override
		{
			assert(PtrNode);
			PtrNode = PtrNode->Next;
			return *this;
		}

		CListConstIterator& operator--() noexcept override
		{
			if (PtrNode)
			{
				PtrNode = PtrNode->Prev;
			}
			else
			{
				PtrNode = PtrRecovery;
			}
			return *this;
		}

		const DataType& operator*() const
		{
			assert(PtrNode);
			return PtrNode->Data;
		}
	};

	class CListConstReverseIterator final : public CListBaseIterator
	{
	public:
		CListConstReverseIterator(node* Node, node* Back) : CListBaseIterator(Node, Back) {}
		CListConstReverseIterator(const CListReverseIterator& Iter) : CListBaseIterator(Iter.PtrNode, Iter.PtrRecovery) {}
		virtual ~CListConstReverseIterator() {}

		CListConstReverseIterator& operator++() noexcept override
		{
			assert(PtrNode);
			PtrNode = PtrNode->Prev;
			return *this;
		}

		CListConstReverseIterator& operator--() noexcept override
		{
			if (PtrNode)
			{
				PtrNode = PtrNode->Next;
			}
			else
			{
				PtrNode = PtrRecovery;
			}
			return *this;
		}

		const DataType& operator*() const
		{
			assert(PtrNode);
			return PtrNode->Data;
		}
	};

public:
	using iterator = CListIterator;
	using reverse_iterator = CListReverseIterator;
	using const_iterator = CListConstIterator;
	using const_reverse_iterator = CListConstReverseIterator;

public:
	CList() {}
	CList(std::initializer_list<DataType> il) 
	{
		for (auto& i : il)
		{
			push_back(i);
		}
	}
	~CList() 
	{
		clear();
	}

	void push_back(DataType Value)
	{
		node* new_node{ new node(Value) };
		
		if (!m_Front)
		{
			m_Front = m_Back = new_node;
		}
		else
		{
			m_Back->Next = new_node;
			new_node->Prev = m_Back;
			m_Back = new_node;
		}

		++m_Size;
	}

	void push_front(DataType Value)
	{
		node* new_node{ new node(Value) };

		if (!m_Front)
		{
			m_Front = m_Back = new_node;
		}
		else
		{
			m_Front->Prev = new_node;
			new_node->Next = m_Front;
			m_Front = new_node;
		}

		++m_Size;
	}

	void pop_back()
	{
		if (!m_Back) return;
		
		if (!m_Back->Prev)
		{
			SAFE_DELETE(m_Back);
			m_Front = nullptr;
		}
		else
		{
			m_Back = m_Back->Prev;
			SAFE_DELETE(m_Back->Next);
		}

		--m_Size;
	}

	void pop_front()
	{
		if (!m_Front) return;

		if (!m_Front->Next)
		{
			SAFE_DELETE(m_Front);
			m_Back = nullptr;
		}
		else
		{
			m_Front = m_Front->Next;
			SAFE_DELETE(m_Front->Prev);
		}

		--m_Size;
	}

	void insert(const_iterator Iter, DataType Value)
	{
		if (Iter.PtrNode)
		{
			if (Iter.PtrNode->Prev)
			{
				node* new_node{ new node(Value) };
				new_node->Prev = Iter.PtrNode->Prev;
				new_node->Next = Iter.PtrNode;
				Iter.PtrNode->Prev->Next = new_node;
				Iter.PtrNode->Prev = new_node;

				++m_Size;
			}
			else
			{
				push_front(Value);
			}
		}
		else
		{
			push_back(Value);
		}
	}

	void erase(const_iterator Iter)
	{
		assert(Iter.PtrNode);

		if (!Iter.PtrNode->Prev && !Iter.PtrNode->Next)
		{
			pop_back();
		}
		else if (!Iter.PtrNode->Prev)
		{
			pop_front();
		}
		else if (!Iter.PtrNode->Next)
		{
			pop_back();
		}
		else
		{
			Iter.PtrNode->Prev->Next = Iter.PtrNode->Next;
			Iter.PtrNode->Next->Prev = Iter.PtrNode->Prev;
			SAFE_DELETE(Iter.PtrNode);

			--m_Size;
		}
	}

	bool empty()
	{
		return (m_Front == nullptr) ? true : false; 
	}

	void clear()
	{
		while (!empty())
		{
			pop_back();
		}
	}

	DataType& front()
	{
		assert(m_Front);
		return m_Front->Data;
	}

	DataType& back()
	{
		assert(m_Back);
		return m_Back->Data;
	}

	size_t size()
	{
		return m_Size;
	}

public:
	iterator begin() { return iterator(m_Front, m_Back); }
	iterator end() { return iterator(nullptr, m_Back); }
	const_iterator begin() const { return const_iterator(m_Front, m_Back); }
	const_iterator end() const { return const_iterator(nullptr, m_Back); }
	reverse_iterator rbegin() { return reverse_iterator(m_Back, m_Front); }
	reverse_iterator rend() { return reverse_iterator(nullptr, m_Front); }
	const_reverse_iterator rbegin() const { return reverse_iterator(m_Back, m_Front); }
	const_reverse_iterator rend() const { return reverse_iterator(nullptr, m_Front); }
	const_iterator cbegin() const { return begin(); }
	const_iterator cend() const { return end(); }
	const_reverse_iterator crbegin() const { return rbegin(); }
	const_reverse_iterator crend() const { return rend(); }

private:
	node* m_Front{};
	node* m_Back{};
	size_t m_Size{};
};