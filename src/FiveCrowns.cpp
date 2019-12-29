/**/
/*********************************************************************************
** Name: Abhash Panta
** Project: Five Crowns (C++)
** Class: CMPS 366-01
** Date: 10/01/2019
**********************************************************************************/
/**/

// FiveCrowns.cpp : Defines the entry point for the console application.
// Main client function that runs the five crowns game


#include "stdafx.h"
#include "Game.h"
#include "Hand.h"
#include "HandAnalyzer.h"

int main()
{
	Game* gameClient = new Game();
	gameClient->startGame();

	return EXIT_SUCCESS;
}

