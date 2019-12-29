/**/
/*********************************************************************************
** Name: Abhash Panta
** Project: Five Crowns (C++)
** Class: CMPS 366-01
** Date: 10/01/2019
**********************************************************************************/
/**/
#pragma once
#include "Player.h"
#include "Round.h"


class Human : public Player
{
public:

	// Constructors
	Human(){}
	Human(string name, unsigned playerNum) : Player(name, playerNum) 
	{
		this->m_hand.getHand().clear();
		this->m_isHuman = true;
		this->m_score = 0;
		this->m_goOutFlag = false;
	};

	// Destructor
	~Human() {};


	//Utility Functions
	void printMenu();
	void play(vector<Card> &drawPile, vector<Card> &discardPile);
	unsigned getMenuInput();
	void askForCall(char &choice);
	void pickCard(vector<Card> &drawPile, vector<Card> &discardPile);
	void dropCard(vector<Card> &discardPile);
	void pickFromDraw(vector<Card> &drawPile);
	void pickFromDiscard(vector<Card> &discardPile);
	void help(vector<Card> drawPile, vector<Card> discardPile);
	void helpPick(vector<Card> discardPile);
	void helpDrop();
	void helpArrange();
	void lastTurn(vector<Card> &drawPile, vector<Card> &discardPile);
};

