#pragma once

#ifndef LIST_H
#define LIST_H

template <class T>
class ListNode
{
private:
	T data;
	ListNode<T>* prev;
	ListNode<T>* next;

public:
	ListNode(T v);
	ListNode(T v, ListNode<T>* p, ListNode<T>* n);
	
	T getVal() const;
	ListNode<T>* getPrev() const;
	ListNode<T>* getNext() const;

	void setVal(T v);
	void setPrev(ListNode<T>* n);
	void setNext(ListNode<T>* n);
};

template <class T>
class List
{
private:
	ListNode<T>* head;
	ListNode<T>* tail;
	ListNode<T>* current;
public:
	List();
	~List();

	bool isEmpty() const;
	ListNode<T>* getCurrentNode() const;

	void setCurrentNode(ListNode<T>* n);

	void push_back(T data);
	void pop_back();
};

#endif // !LIST_H