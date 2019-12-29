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

class Hand
{
public:
	// Constructor
	Hand() 
	{
		this->m_hand.clear();
		this->m_handSize = 0;
	}
	// Destructor
	~Hand(){}

	// Accessor Function
	inline vector<Card> getHand() const
	{
		return this->m_hand;
	}

	inline unsigned getSize() const
	{
		return this->m_handSize;
	}
	
	
	// Mutator Function
	inline void setHand(vector<Card> hand)
	{
		this->m_hand = hand;
		this->m_handSize = hand.size();
	}

	inline void setHandSize(unsigned size)
	{
		this->m_handSize = size;
	}

	// Public Utility Functions
	void displayHand();
	void clearHand();
	void addCardToHand(Card c);
	Card removeCard(string card_string);

private:
	vector<Card> m_hand;
	unsigned m_handSize;
};

