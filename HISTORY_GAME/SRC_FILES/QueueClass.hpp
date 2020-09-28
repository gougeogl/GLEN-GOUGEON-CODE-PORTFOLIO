/*****************************************************************************
 * File Name: QueueClass.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Queue Class [DATA STRUCTURE]:

 *****************************************************************************/
#ifndef QUEUE_CLASS_HPP
#define QUEUE_CLASS_HPP

#include<iostream>
using std::cout;
using std::cin;
using std::endl;

#include<string>
using std::string;

#include"item.hpp"

class Queue
{
	private:	
			struct QueueNode
			{
				Item *object;
				QueueNode *next;
				QueueNode *previous;

				QueueNode(Item *someItem, QueueNode *prev1 = nullptr, QueueNode *next1 = nullptr)
				{
					object = someItem;
					previous = prev1;
					next = next1;
				}

				~QueueNode()
				{
					object = nullptr;
					previous = nullptr;
					next = nullptr;
				}
			};
			QueueNode *head;
			int Qsize;
	public:
			Queue();
			~Queue();

			int getQSize();
			bool isEmpty(); 
						
			// return item at front of queue
			Item* getFront();

			// place item at 'back' of queue
			void enqueue(Item *);

			 // Look (only) at the Item* in head 
			 //DOES NOT return Item*
			string peekSatchel(); 

			// search specifically
			Item* searchSatchel(string);

			void rotateSatchel(); 

			void printQueue();

};
#endif

//***************************** E O F ********************************

