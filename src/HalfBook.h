/**/
/*********************************************************************************
** Name: Abhash Panta
** Project: Five Crowns (C++)
** Class: CMPS 366-01
** Date: 10/01/2019
**********************************************************************************/
/**/
#pragma once
#include "Combination.h"

class HalfBook : public Combination
{
public:
	// Constructors
	HalfBook() {}
	HalfBook(vector<Card> pile) : Combination(pile) {};

	// Destructor
	~HalfBook(){}

	// Utility Functions
	inline bool isIncomplete()
	{
		return true;
	}

	void printPile();

};

