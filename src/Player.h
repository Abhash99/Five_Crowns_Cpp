/**/
/*********************************************************************************
** Name: Abhash Panta
** Project: Five Crowns (C++)
** Class: CMPS 366-01
** Date: 10/01/2019
**********************************************************************************/
/**/

#ifndef PLAYER_H
#define PLAYER_H


#include "stdafx.h"
#include "Hand.h"
#include "HandAnalyzer.h"

class Player
{
public:
	// Constructor for player class
	Player() 
	{
		this->m_goOutFlag = false;
		this->m_hand.getHand().clear();
		this->m_isHuman = false;
		this->m_playerNum = 0;
		this->m_score = 0;
	};
	Player(string name, unsigned playerNum);

	// Destructor
	~Player() {}

	// Accessors
	inline string getName() const
	{
		return this->m_name;
	}

	inline unsigned getPlayerNum() const
	{
		return this->m_playerNum;
	}

	inline unsigned getScore() const
	{
		return this->m_score;
	}


	inline Hand getHand() const
	{
		return this->m_hand;
	}

	inline bool isHuman() const
	{
		return this->m_isHuman;
	}

	inline bool wentOut() const
	{
		return m_goOutFlag;
	}


	//Mutators
	inline void setName(string name)
	{
		this->m_name = name;
	}

	inline void setPlayerNum(unsigned number)
	{
		this->m_playerNum = number;
	}

	inline void setScore(unsigned score)
	{
		this->m_score = score;
	}

	inline void setHand(Hand hand)
	{
		this->m_hand = hand;
	}

	inline void setIsHuman(bool isHuman)
	{
		this->m_isHuman = isHuman;
	}

	inline void setGoOutFlag(bool goOut)
	{
		this->m_goOutFlag = goOut;
	}

	// Utility Functions
	virtual void printMenu(){}
	virtual unsigned getMenuInput() { return 1;}
	virtual void play(vector<Card> &drawPile, vector<Card> &discardPile);
	virtual void askForCall(char &choice){}


	void clearHand();

	void addCard(Card c);
	virtual void pickFromDraw() {}
	virtual void pickFromDiscard() {}
	void goOut();
	virtual void lastTurn(vector<Card> &drawPile, vector<Card> &discardPile) {}
	void evaluateHand();

protected: 
	string m_name;
	unsigned m_playerNum;
	unsigned m_score;
	Hand m_hand;
	bool m_isHuman;
	bool m_goOutFlag;
};
#endif // !PLAYER_H

