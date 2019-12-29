#include "stdafx.h"
#include "Deck.h"


/*****************************************************************************************************************
/* Function Name: initializeDeck 
/* Purpose: To generate a new deck of 58 cards (including jokers)
/* Parameters: None
/* Return Value: None
/* Local Variables: 
					Card* newCard is used to point to every new card that is created. 
/* Algorithm:
			Clear the deck. Create new refular cards and insert them into the deck. Also, create three jokers and 
			insert them into the deck. 
/* Assistance Received: None

******************************************************************************************************************/
void Deck::initializeDeck()
{
	m_deck.clear();
		// Load all the cards in the deck (not including the joker)
		for (unsigned suit = 1; suit <= Card::M_NUM_SUITS; suit++)
		{
			for (unsigned rank = 3; rank <= Card::M_NUM_RANKS; rank++)
			{
				Card* newCard = new Card(suit, rank);
				m_deck.push_back(*newCard);
				this->m_size++;
			}
		}

		// Load all the jokers in the deck
		for (unsigned i = 1; i <= 3; i++)
		{
			Card* newCard = new Card(6, i);
			m_deck.push_back(*newCard);
			this->m_size++;
		}
}

/*****************************************************************************************************************
/* Function Name: shuffle
/* Purpose: To randomly shuffle all the cards in the decks
/* Parameters: None
/* Return Value: None
/* Local Variables: None
/* Algorithm: 
			We seed the random number generator and use the random_shuffle function from <algorithm> library to 
			randomly shuffle our vector of cards (deck). 
/* Assistance Received:

******************************************************************************************************************/
// Randomly shuffles the deck
void Deck::shuffle()
{
	srand(unsigned(time(0)));
	random_shuffle(m_deck.begin(), m_deck.end());
}

/*****************************************************************************************************************
/* Function Name: printDeck
/* Purpose: To print all the cards in the deck
/* Parameters: None
/* Return Value: None
/* Local Variables: None
/* Algorithm: Iteratively prints all the cards in the deck in their string representations. 
			We don't use this in our program but this functionality can be used if every needed. 
/* Assistance Received: None

******************************************************************************************************************/
// Prints Deck
void Deck::printDeck() 
{
	for (auto card : m_deck)
	{
		cout << card.toString() + " ";
	}

	cout << endl;
}
