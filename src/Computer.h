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
#include "HandAnalyzer.h"

class Computer : public Player
{
public:
	// Constructors
	Computer(){}

	// Parametrized Constructor
	Computer(string name, unsigned playerNum) : Player(name, playerNum) 
	{
		this->m_hand.getHand().clear();
		this->m_isHuman = false;
		this->m_score = 0;
		this->m_goOutFlag = false;
	};
	
	// Destructors
	~Computer(){}
	

	// Utitlity Functions
	void printMenu();
	unsigned getMenuInput();
	void play(vector<Card> &drawPile, vector<Card> &discardPile);

	void askForCall(char &choice);

	void pickCard(vector<Card> &drawPile, vector<Card> &discardPile);
	void dropCard(vector<Card> &discardPile);
	Card cardToDrop();

	void pickFromDraw(vector<Card> &drawPile);
	void pickFromDiscard(vector<Card> &discardPile);

	bool canGoOut();
	void lastTurn(vector<Card> &drawPile, vector<Card> &discardPile);
	static Card findGreatestCard(vector<Card> cardStack);

};

