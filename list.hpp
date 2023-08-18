#include<iostream>

#ifndef LIST_H
#include "list.h"
#endif

using namespace std;

// Node implementation
template <class T>
ListNode<T>::ListNode(T v)
{
    val = v;
    prev = nullptr;
    next = nullptr;
}

template <class T>
ListNode<T>::ListNode(T v, ListNode<T>* p, ListNode<T>* n)
{
	val = v;
	prev = p;
	next = n;
}

template <class T>
T ListNode<T>::getVal() const
{
    return val;
}

template<class T>
ListNode<T>* ListNode<T>::getPrev() const
{
    return prev;
}

template<class T>
ListNode<T>* ListNode<T>::getNext() const
{
    return next;
}

template <class T>
void ListNode<T>::setVal(T v)
{
    val = v;
}

template<class T>
void ListNode<T>::setPrev(ListNode<T>* n)
{
    prev = n;
}

template<class T>
void ListNode<T>::setNext(ListNode<T>* n)
{
    next = n;
}

// List implementation
template <class T>
List<T>::List()
{
	head = nullptr;
	tail = nullptr;
	current = nullptr;
}

template <class T>
List<T>::~List<T>()
{
	ListNode<T>* temp = head;

	while (temp != nullptr)
	{
		ListNode<T>* next = temp->next;
		delete temp;
		temp = next;
	}
}

template <class T>
bool List<T>::isEmpty() const
{
	return head == nullptr;
}

template <class T>
ListNode<T>* List<T>::getCurrentNode() const
{
	return current;
}

template <class T>
void List<T>::setCurrentNode(ListNode<T>* n)
{
	current = n;
}

template <class T>
void List<T>::push_back(T data)
{
	if (head == nullptr)
	{
		head = new ListNode<T>(data);
		tail = head;
		current = head;
	}
	else
	{
		ListNode<T>* temp = new ListNode<T>(data, tail, nullptr);
		tail->next = temp;
		tail = temp;
	}
}

template <class T>
void List<T>::pop_back()
{
	// empty list
	if (head == nullptr)
	{
		return;
	}
	// one element
	else if (head == tail)
	{
		delete head;
		head = nullptr;
		tail = nullptr;
	}
	else
	{
		ListNode<T>* temp = tail;
		tail = tail->prev;
		tail->next = nullptr;
		delete temp;
	}
}