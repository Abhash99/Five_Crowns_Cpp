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
#include "Combination.h"

class Book : public Combination
{
public:
	// Constructors and Destructor
	Book(){}
	Book(vector<Card> pile) : Combination(pile) {}
	Book::~Book(){}



	// Utility Functions
	inline bool isIncomplete()
	{
		return false;
	}

	void printPile();
};

