/*****************************************************************************
 * File Name: UntouchablesOffice_Level.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Untouchables Office Class (SUB) Level
	
	SEE: action() for Level description.

 *****************************************************************************/
#include"UntouchablesOffice_Level.hpp"

// Default Constructor
Office::Office()
{
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;

	this->isDiscovered();

	// fill level with items
	itemStack->push(myInventory->fedora());
	itemStack->push(myInventory->shotgun());
	itemStack->push(myInventory->AugustDecrees());
	itemStack->push(myInventory->hamburger());
	itemStack->push(myInventory->torch());

	// Untouchables Office
	this->setLocationName("\n   1930 Chicago, USA.\n   Bureau of Prohibition.\n");
	this->setDate("June", 1, 1930);
	this->setDescription("\n   Office of Special Agent in Charge, Elliot Ness.\n");

	string explanation = "\n   In 1930, U.S. District Attorney George E. Q. Johnson devised a two-pronged"
		"\n   plan to \"get at Capone\" per President Hoover\'s request. Elliot Ness and his band of \"Untouchables\""
		"\n   would attack Capone\'s alcohol, and Treasury agents Elmer Irey and Frank Wilson would attack his tax records.";

	this->setSignificance(explanation);

	introRead = false;
}

// Destructor
Office::~Office() {}

/**********************************************************************************
Function Name: search()
Description:
		Performs action SEARCH option on a Level

		STEPS:
			1. Prompt yes or no input to search
			2. Validates step 1 input
			3. If yes:
				AND itemStack is empty:
					prints indicate so
				AND itemStack is NOT empty:

					a) pop item off 'itemStack' (BASE class member of LevelClass)
					b) print item popped
					c) ask if user wants it y/Y/n/N only (validates)
					d) If doesn't want:
						push onto 'droppedItems' Stack (BASE class member of LevelClass)
					e) If wants:
						enqueue in hero's 'satchel' (queue)

			4. If no: exit SEARCH

Parameters: Character* hero (YOU in game)

Effect:
		Gain item popped or it is left for recovery later

*************************************************************************************/
void Office::search(Character* hero)
{
	cout << "\n   Should I be looking for something here?"
		 << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	// validate
	char password = checkYesNo("Why can't I make up my mind?");

	if (toupper(password) == 'Y')
	{
		if (itemStack->isEmpty())
		{
			printThenEnter("There\'s nothing left in the office worth having.");
		}
		else
		{
			Item *temp = nullptr;
			temp = itemStack->pop();

			wipeScreen();

			// displays what was found w/ custom dialogue
			this->printNewItemFound(temp, "what\'s this here? looks like");

			// validate
			char response = checkYesNo("Ah.. Mr. Ness? Sir?");

			// leaving behind at this location
			if (toupper(response) == 'N')
			{
				printThenEnter("hmm..   I don't want that now.");

				// this will place it in a queue at this location
				this->droppedItems->enqueue(temp);
			}
			else
			{
				// place in satchel
				hero->getSatchel()->enqueue(temp);
				// show effect
				this->printUpdatedQueue(hero);
			}

		}
	}

}

/*********************************************************************************
PURE VIRTUAL Function Name: action()
Description:
	Executes ACTION cmd option for this level. If hero can deliver Al Capone's
	accounting ledger to Frank Wilson, you WIN THE GAME! The ledger is only 
	available by conducting a raid on one of Capone's liquor operations.

**********************************************************************************/
void Office::action(Character* hero)
{
	cin.ignore();

	// plotline
	cout << "\n   LOCATION:";
	cout << this->getLocationName();

	// INTRODUCTION
	if (!introRead) { untouchablesIntroDialogue(); }
	if (introRead)
	{
		Bourgeoisie* Irey = new Bourgeoisie();
		Irey->setName("Elmer Irey");

		Bourgeoisie* Wilson = new Bourgeoisie();
		Wilson->setName("Frank Wilson");

		printEnterWipe("Knock-Knock!");
		printEnterWipe("Secretary: \"Ah.. Mr. Ness.. the men from the Treasury Dept. are here to see you.\"");
		printEnterWipe("You open the door to find ..");

		// print men
		cout << "\n";
		Irey->printStats();
		cout << "\n\n   - AND -\n\n";
		Wilson->printStats();
		printEnterWipe(" ");

		printEnterWipe("Hey Ness! Look. We know you are busy but..");
		printEnterWipe("We could use a break. You know that ledger we were talking about?");
		printEnterWipe("There must be a copy of it somewhere else!");
		
		cout << "\n   Have you found it?"
			<< "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

		// validate
		char password = checkYesNo("You know! It\'ld have an accounting of all his dealings.");

		if (toupper(password) == 'Y')
		{
			printEnterWipe("Fantastic!");
			if (hero->iHaveThis("\n   Al Capone\'s accounting ledger"))
			{
				Item* ledger = hero->getSatchel()->searchSatchel("\n   Al Capone\'s accounting ledger");
				printEnterWipe(" ");

				// give ledger to Frank Wilson
				printEnterWipe("Wilson: \"I\'ll take that Ness. Thanks.\"");
				Wilson->getSatchel()->enqueue(ledger);

				// print Wilson's satchel (queue)
				printUpdatedQueue(Wilson);
				wipeScreen();

				printEnterWipe("Irey: \"The D.A.\'s gonna LOVE this!\"");
				printEnterWipe("Al Capone was eventually convicted and sentenced to 11 years in prision for tax evasion.");
				printEnterWipe("Frank Wilson the under-rated Treasury agent, Elliot Ness\' team of \'Untouchables\'..");
				printEnterWipe("Methodical investigations, witness protection, and persistence finally put Capone behind bars.");
				printEnterWipe("He served time in a U.S. penitentary in Atlanta, GA. as well as Alcatraz.");
				printEnterWipe("Al Capone only served 7 years, 6 months, and 15 days of his sentence..");
				printEnterWipe("since he contracted neurosyphilis, and eventually dimentia.");
				printEnterWipe("He died at his posh home in Florida with the apparent intellect of a 12-year-old-boy on:");
				printEnterWipe("January 25, 1947.");

				printEnterWipe("Good Job delivering that evidence!");
				printEnterWipe("YOU WON THE GAME !!");
				this->setWonGame();
			}
			else
			{
				printEnterWipe("C\'mon Ness.. don\'t tease us like that!");
				printEnterWipe("Well ..next time you make a raid.. just keep your eyes peeled alright?");
			}
		}
		else
		{
			wipeScreen();
			cin.ignore();
			printEnterWipe("Well ..next time you make a raid.. just keep your eyes peeled alright?");
		}

		delete Irey; Irey = nullptr;
		delete Wilson; Wilson = nullptr;
	}
}

/***********************************************************
Function: untouchablesIntroDialogue()

Description:
	Introduction to Historical context. Prints theoretical
	'wrap-sheet' of Al-Capone. The events are historically
	correct, but the existence of such a document is not.

************************************************************/
void Office::untouchablesIntroDialogue()
{
	printDate();
	cout << "\n   DESCRIPTION:";
	cout << this->getDescription();
	cout << "\n\n   SIGNIFICANCE:";
	cout << this->getSignificance();
	printEnterWipe(" ");

	printEnterWipe("Looking around, you see that you\'re in an office.");
	printEnterWipe("The name on the desk is Ness, and the print on the door says:");
	printEnterWipe("SAC Elliot Ness. Dept. of Prohibition.");
	printEnterWipe("Kind of a no-brainer.");
	printEnterWipe("The calendar is on June.. 1930 and there\'s a stack of papers on the desk.");
	printEnterWipe("Same month Al Capone went to trial if I remember right.");
	printEnterWipe("Ness\' job was a bust and publicize campaign if I\'m not mistaken.");
	printEnterWipe("You see a file:");
	printEnterWipe("Dept. of Prohibition. For offical use only.");
	printEnterWipe("\'Capone, Alphonso\'");
	cout << "\n   ==========================================================================================="
		<< "\n   Dept. of Prohibition 1930."
		<< "\n   Subject Details."
		<< "\n\n   last name: Capone"
		<< "\n   first name: Alphonso"
		<< "\n   "
		<< "\n   residence:"
		<< "\n   93 Palm Ave."
		<< "\n   Miami Beach, FL."
		<< "\n   ----------------"
		<< "\n   Subject History."
		<< "\n\n   Chicago, IL. 1920. Driving under the influence. Subject crashed"
		<< "\n		into taxi-cab. Direct evidence. All Charges dropped."
		<< "\n\n   Cicero, IL. Mar 31, 1924. Suspected involvement with election tampering."
		<< "\n		Homicide and blackmail of voters. Frank Capone shot in street. No evidence. No charges."
		<< "\n\n   1925. Johnny Torrio suspect associated with crime boss. Torrio at large abroad."
		<< "\n		Illegal alcohol trafficking, prostitution, and gambling."
		<< "\n		Capone inherited Torrio\'s organization named the \'outfit\' in absence."
		<< "\n\n   1926. Cicero, IL. Known rivals of Capone found dead. \'Hangin\' prosecutor"
		<< "\n		William McSwiggin also killed in attack. McSwiggin\'s evidence suggests"
		<< "\n		Capone ordered hit."
		<< "\n\n   Feb. 14, 1929. Known rival \"Bugs\" Malone hitmen murdered in northside garage."
		<< "\n		8 identified as Malone\'s enforcers. D.O.A. No charges filed."
		<< "\n		Alibi-ed out in Florida residence at time. No evidence. No Charges filed."
		<< "\n   ..."
		<< "\n   ===========================================================================================";
	printEnterWipe(" ");

	introRead = true;
}

//***************************** E O F ********************************
