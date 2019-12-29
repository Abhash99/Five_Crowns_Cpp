/**/
/*********************************************************************************
** Name: Abhash Panta
** Project: Five Crowns (C++)
** Class: CMPS 366-01
** Date: 10/01/2019
**********************************************************************************/
/**/

#pragma once
#include "stdafx.h"
#include "Card.h"

class Deck
{
public:

	static const unsigned MAX_DECK_SIZE = 58;

	// Default Constructor
	Deck()
	{
		this->m_size = 0;
	}

	// Parametrized Constructor
	Deck(vector<Card> deck, unsigned size)
	{
		this->m_deck = deck;
		this->m_size = size;
	}

	// Destructor
	Deck::~Deck(){}

	// Accessors
	inline vector<Card> getDeck() const
	{
		return this->m_deck;
	}

	inline unsigned getSize() const
	{
		return this->m_size;
	}

	// Mutators
	inline void setDeck(vector<Card> deck)
	{
		this->m_deck = deck;
	}

	inline void setSize(unsigned size)
	{
		this->m_size = size;
	}


	// Utility Functions
	void initializeDeck();
	void shuffle();
	void printDeck();

private:
	vector<Card> m_deck;
	unsigned m_size;


};

