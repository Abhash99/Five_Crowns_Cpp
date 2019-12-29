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

class Run : public Combination
{
public:
	// Constructors
	Run(){}

	Run(vector<Card> pile) : Combination(pile) {};

	// Destructor
	~Run() {};

	// Utility Functions
	inline bool isIncomplete()
	{
		return false;
	}

	void printPile();
};

