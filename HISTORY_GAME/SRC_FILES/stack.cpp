/*****************************************************************************
 * File Name: stack.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Stack Class [DATA STRUCTURE]:

 *****************************************************************************/
#include "stack.hpp"

// Constructor
Stack::Stack() { top = nullptr; }

// Destructor
Stack::~Stack()
{
	Item* trash = nullptr;

	while (!this->isEmpty())
	{
		trash = this->pop();
		delete trash;
		trash = nullptr;
	}
}

// pushes an Item* thing onto the stack
void Stack::push(Item *thing)
{
	top = new StackNode(thing, top);
}

// returns the item popped off the top of the stack
Item* Stack::pop()
{
	StackNode *temp = nullptr;
	Item *thing = nullptr;

	if (this->isEmpty())
	{
		cout << "\n   Nothing found to pop.";
	}
	else
	{
		// pop value off the top
		thing = top->item;
		temp = top;
		top = top->next;
		delete temp;
	}

	return thing;
}

// evaluates if the stack is empty
bool Stack::isEmpty() const
{
	bool verdict = false;

	if (top == nullptr)
	{
		verdict = true;
	}

	return verdict;
}
