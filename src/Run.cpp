#include "stdafx.h"
#include "Run.h"

/*****************************************************************************************************************
/* Function Name: printPile
/* Purpose:	To print all the cards in the combination (run)
/* Parameters: None
/* Return Value: None
/* Local Variables:	None
/* Algorithm: If there are more than two cards in the combination, print the cards in the pile iteratively.
/* Assistance Received: None

******************************************************************************************************************/
void Run::printPile()
{
	if (getPile().size() > 2)
	{
		cout << " make a run with ";

		for (auto card : getPile())
		{
			cout << card.toString() << " ";
		}
		cout << "." << endl;
	}
	
}