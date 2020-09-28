/*****************************************************************************
 * File Name: QueueClass.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Queue Class [DATA STRUCTURE]:

 *****************************************************************************/
#include"QueueClass.hpp"

// default constructor
Queue::Queue()
{
	head = nullptr;
	Qsize = 0;
}

/************************************************
Function Name: ~Queue() DESTRUCTOR
EXPLANATION:
	getFront() is a QueueClass member function
	that returns a pointer to the Item* held 
	in each QueueNode's 'object' variable.
	Since the QueueNode has its own destructor
	defined in QueueClass.hpp.. you only need
	to set nullptr on the QueueNode members..
	and the ~QueueNode() DESTRUCTOR will be 
	called as it goes out of scope.

	BUT you MUST call delete on the object
	itself below or you WILL get a memory leak!

*************************************************/
Queue::~Queue()
{
	while (!this->isEmpty()) 
	{ 
		delete this->getFront(); 
	}	
}

/**************************************************************
* Function Name: getQSize()
* Description:
		Returns the quantity of items in the queue.

***************************************************************/
int Queue::getQSize() { return Qsize; }

/**************************************************************
* Function Name: isEmpty()
* Description:
		Helper. Used to check if the queue is currently empty. 
		It does so by checking if head is set to nullptr. 

* Returns: 
	if head equals 'nullptr' : TRUE
	otherwise : FALSE

***************************************************************/
bool Queue::isEmpty()
{
	bool result = false;	

	if (head == nullptr)
	{
		result = true;
	}

	return result;
}

/******************************************************************************
* Function Name: getFront()
* Description:
		returns the Item* object held in Head of Queue

	STEPS:
		1. call isEmpty() to check queue is not empty
		2. locally save object member of head (an Item*)
		3. re-connect links/nodes of queue
		4. update Queue size
		5. return Item*

*******************************************************************************/
Item* Queue::getFront()
{
	Item* last = nullptr;

	if (!this->isEmpty())
	{
		last = head->object;

		if (head->next == head && head->previous == head)
		{
			delete head;
			head = nullptr;
		}
		else
		{
			QueueNode* temp = head;
			QueueNode* newHead = head->next;

			// move pointers
			head->previous->next = head->next;
			head->next->previous = head->previous;

			// delete old head .. call ~QueueNode()
			delete temp; temp = nullptr;

			// setup new head
			head = newHead;
		}
		// update size
		this->Qsize--;
	}
	return last;
}

/******************************************************************************
* Function Name: enqueue()
* Description:
		Enqueues new Item at back of the circular queue.

* Parameters:
		Item* relic. The new Item to add to the back of the queue.

* Conditions:
		1. Head is empty
		2. Head is not empty

* Effect:
		1.	If more than 1 item in queue previously:
				'relic' placed in back of queue is head-previous
			If previously empty
				'relic' is ONLY item in queue

		2.	Queue size incremented

*******************************************************************************/
void Queue::enqueue(Item *relic) 
{
	if (this->isEmpty())
	{

		head = new QueueNode(relic);
		head->previous = head;
		head->next = head;
	}
	else
	{
		head->previous->next = new QueueNode(relic, head->previous, head);
		head->previous = head->previous->next;
	}
	this->Qsize++;
}

/******************************************************************************
* Function Name: peekSatchel()
* Description:
		A satchel is a bag. Each Character* carries one. The imagery is of
		peeking inside the flap of this bag one would carry.

		This returns a string description of the Item at the 
		'front' of the queue (head)

	STEPS:
		1. call isEmpty() to check queue is not empty
		2. If empty print empty
		3. If not empty call getTitle() on head object (an Item*)
		4. return string

*******************************************************************************/
string Queue::peekSatchel() 
{
	string whatIs = " ";
	
	if(this->isEmpty())
	{
		cout << "\n   Looks like my satchel is empty."
			 << "\n   Time to search for something else.\n";
	}	
	else 
	{
		whatIs = head->object->getTitle();
	}
	
	return whatIs;
}

/*******************************************************************
Function: searchSatchel()
Description:
	Locates 'searchingFor' in Queue

Parameters:
	string searchingFor. The EXACT string of the Item's name str

Returns: Item* (item) sought by str 'searchingFor'

********************************************************************/
Item* Queue::searchSatchel(string searchingFor)
{
	bool found = false;
	Item* item = nullptr;

	// empty satchel
	if(this->isEmpty()){ cout << "\n   Your satchel was empty."; }

	// not empty
	else if (!this->isEmpty())
	{
		int counter = this->getQSize();
		do
		{
			if (this->peekSatchel() == searchingFor)
			{
				found = true;
				item = this->getFront();

				cout << "\n   Found: ";
				item->printItem();

				break;
			}
			else if (this->peekSatchel() != searchingFor)
			{
				this->rotateSatchel();
			}
			counter--;

		} while (!found || counter == 0);

	}

	return item;
}

/*******************************************************************
Function: rotateSatchel()
Description:
	Moves Item* in Head->object to back of the queue

	STEPS:
		1. If satchel is empty, print to user
		2. If not empty call getFront()
		3. Store return of step 2
		4. call enqueue with step 3 output as input

	NOTE*: DOES NOT ADJUST QSize !! (no need..just rotating stock)

********************************************************************/
void Queue::rotateSatchel() 
{
	if (this->isEmpty())
	{
		cout << "\n   Empty satchel. Nothing to grab.\n";
	}
	else
	{	// create temporary ptr
		Item *thing = nullptr;
		
		// set it to what's in front
		thing = this->getFront();
		
		// put champion at back of queue
		this->enqueue(thing);
		thing = nullptr;
	}

}

/*******************************************************************
Function: printQueue()
Description:
		Prints the contents of the queue starting from the head, until the theoretical tail.
		Remember: it is however, a circular queue list.
	
	STEPS:
		1. If queue empty print empty
		2. If not empty copy head to temp
		3. call printItem() on temp
		4. set temp = temp->next
		5. repeat steps 3-4 while 
				temp doesn't equal head 
					-AND-
				temp isn't nullptr

Effect:		Contents of a Queue are nicely printed in game

********************************************************************/
void Queue::printQueue()
{
	if (this->isEmpty())
	{
		cout << "\n   Satchel empty. I must've worn a whole in this bag.\n";
	}
	else	
	{
		QueueNode *temp = head;
		
		do	
		{	
			temp->object->printItem();
			temp = temp->next;

		} while(temp != head && temp != nullptr);

		temp = nullptr;
	}

}

//***************************** E O F ********************************

