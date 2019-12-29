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

class HalfRun : public Combination
{
public:
	//Constructor
	HalfRun(){}
	HalfRun(vector<Card> pile) : Combination(pile) {};

	// Destructor
	~HalfRun(){}

	// Utility Functions
	inline bool isIncomplete()
	{
		return true;
	}

	void printPile();

};

