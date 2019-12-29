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

class Combination
{
public:
	// Default Constructor
	Combination()
	{
		this->m_pile.clear();
	}

	// Parametrized Constructor
	Combination(vector<Card> pile)
	{
		this->m_pile = pile;
	}

	// Destructor
	~Combination(){}

	

	// Accessor
	inline vector<Card> getPile() const
	{
		return this->m_pile;
	}

	// Mutators
	inline void setPile(vector<Card> pile)
	{
		this->m_pile = pile;
	}


	//Utility Functions
	inline void addToPile(Card newCard)
	{
		m_pile.push_back(newCard);
	}


	//Utility Functions
	virtual bool isIncomplete() { return false; }
	virtual void printPile() {}

protected:
	vector<Card> m_pile; 
};

