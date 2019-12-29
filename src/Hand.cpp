#include "stdafx.h"
#include "Hand.h"

/*****************************************************************************************************************
/* Function Name: displayHand
/* Purpose: To print all the card in the hand
/* Parameters: None	
/* Return Value: None
/* Local Variables: None	
/* Algorithm:
			For each card in player's hand, convert the card to its string representation using toString()
			and print the cardstring. 
/* Assistance Received: None

******************************************************************************************************************/
void Hand::displayHand()
{
	// If handSize is 0 or less, we print the hand is empty
	if (this->m_hand.size() <= 0)
	{
		cout << "Hand: Empty" << endl;
	}
	else
	{
		cout << "Hand: ";
		for (auto card : m_hand)
		{
			cout << card.toString() + " ";
		}
		cout << endl;

	}
}

/*****************************************************************************************************************
/* Function Name: clearHand
/* Purpose: To clear (remove) all cards in player's hand
/* Parameters: None	
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			Simply use the clear function of vectors to remove all the cards in the current hand.
/* Assistance Received: None

******************************************************************************************************************/
void Hand::clearHand()
{
	this->m_hand.clear();
}

/*****************************************************************************************************************
/* Function Name: addCardToHand
/* Purpose: To add a given card to current hand. 
/* Parameters:
			Card c - the card that is to be added to the hand
/* Return Value: None	
/* Local Variables: None
/* Algorithm: Simply add the card to hand by using push_back function in vectors. Increase hand size. 
/* Assistance Received: None

******************************************************************************************************************/
void Hand::addCardToHand(Card c)
{
	m_hand.push_back(c);
	this->m_handSize++;
}


/*****************************************************************************************************************
/* Function Name: removeCard
/* Purpose: To remove a card from the hand
/* Parameters:
			string card_string - the string representation of the card to be deleted. 
/* Return Value: 
			Returns the card that is removed from the hand. We return the card so that we can use it again.
			(Maybe add it to discard pile after it is removed)
/* Local Variables:
			Card temp - temporary card that will hold the card to be removed. It will be returned at the end of the program. 
/* Algorithm:
			Iterate through the hand to find the card that matches the card string. 
			If it is present, find the card's position and remove the card from hand. 
			Return the removed card. 
/* Assistance Received: None. 

******************************************************************************************************************/
Card Hand::removeCard(string card_string)
{
	Card temp;

	// If the hand is not empty, iterate through the hand to find the card that matches the card string. 
	// Remove the card if it is present in the array and return it. 
	if (!this->getHand().empty())
	{
		for (unsigned i = 0; i < this->m_hand.size(); i++)
		{
			Card now = m_hand[i];
			string s = m_hand[i].toString();
			if (s == card_string)
			{
				temp = m_hand[i];
				break;
			}
		}
		// Get the iterator to point to the card to be deleted. 
		vector<Card>::iterator it = find(m_hand.begin(), m_hand.end(), temp);
		if (it != m_hand.end())
		{
			m_hand.erase(remove(m_hand.begin(), m_hand.end(), temp), m_hand.end());
			this->m_handSize--;
			return temp;
		}
	}
}
