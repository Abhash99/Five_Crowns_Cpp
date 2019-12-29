#include "stdafx.h"
#include "HalfBook.h"

/*****************************************************************************************************************
/* Function Name: printPile
/* Purpose:	To print all the cards in the combination (halfRun)
/* Parameters: None
/* Return Value: None
/* Local Variables:	None
/* Algorithm: If there are more than two cards in the combination, print the cards in the pile iteratively.
/* Assistance Received: None

******************************************************************************************************************/
void HalfBook::printPile()
{
	if (getPile().size() > 2)
	{
		cout << " make a book with ";

		for (auto card : getPile())
		{
			cout << card.toString() << " ";
		}
		cout << "." << endl;
	}
}