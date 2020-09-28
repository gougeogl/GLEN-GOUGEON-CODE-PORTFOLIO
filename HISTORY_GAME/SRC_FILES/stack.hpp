/*****************************************************************************
 * File Name: stack.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Stack Class [DATA STRUCTURE]:

 *****************************************************************************/
#ifndef GGOUGEON_STACK_CLASS_HPP
#define GGOUGEON_STACK_CLASS_HPP

#include<iostream>
#include<cstdlib>

#include"item.hpp"

class Stack
{
	private:
		struct StackNode
		{
			Item *item;
			StackNode *next;

			// Constructor
			StackNode(Item *item1, StackNode *next1 = nullptr)
			{
				item = item1;
				next = next1;
			}

			~StackNode()
			{
				/* DO NOT DELETE ITEM HERE
				 will cause pop() to break; item will be cleaned up
				 in ~Stack() destructor via pop()*/
				item = nullptr;
				next = nullptr;
			}
		};
		StackNode *top;

	public:	
		Stack(); 
		~Stack();

		// member functions 
		void push(Item *);

		// returns top item
		Item* pop(); 

		bool isEmpty() const;

};
#endif 

//***************************** E O F ********************************
