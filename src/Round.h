/**/
/*********************************************************************************
** Name: Abhash Panta
** Project: Five Crowns (C++)
** Class: CMPS 366-01
** Date: 10/01/2019
**********************************************************************************/
/**/

#ifndef ROUND_H
#define ROUND_H

#include "stdafx.h"
#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "Serializer.h"

class Round
{
public:

	// Symbolic Constants
	static const unsigned MAX_ROUND_NUM = 11;

	// Default Constructor
	Round(){}


	// Parametrized Constructors
	Round(vector<Player*> playerList, unsigned round_num, unsigned numPlayers);
	Round(vector<Player*> playerList, unsigned round_num, unsigned numPlayers, vector<Card> drawPile, vector<Card> discardPile, Player* nextPlayer);
	
	// Destructor
	~Round() {}

	//=================================================================================================================
	// Accessors
	inline vector<Player*> getPlayerList()
	{
		return m_playerList;
	}

	inline unsigned getRoundNum()
	{
		return m_round_num;
	}

	inline vector<Card> getDrawPile()
	{
		return m_drawPile;
	}
	
	inline vector<Card> getDiscardPile()
	{
		return m_discardPile;
	}

	inline Player* getNextPlayer()
	{
		return m_nextPlayer;
	}

	inline unsigned getNumPlayers()
	{
		return m_numPlayers;
	}

	//================================================================================
	// Mutators
	void setPlayerList(vector<Player*> playerList)
	{
		m_playerList = playerList;
	}

	void setRoundNum(unsigned round_num)
	{
		m_round_num = round_num;
	}

	void setDrawPile(vector<Card> drawPile)
	{
		m_drawPile = drawPile;
	}

	void setDiscardPile(vector<Card> discardPile)
	{
		m_discardPile = discardPile;
	}

	void setNextPlayer(Player* nextPlayer)
	{
		m_nextPlayer = nextPlayer;
	}
	
	void setNumPlayers(unsigned numPlayers)
	{
		m_numPlayers = numPlayers;
	}

	//=========================================================================================================
	// Utility Functions
	unsigned initiateRound();
	void initializeDrawPile();
	void printRoundInfo();
	void addTodiscardPile(Card c);
	void makeWildcard();
	void saveGame();
	void clearPlayerHands();
	Card pickFromDrawPile();
	Card pickFromDiscardPile();
	void printDrawPile();
	void printDiscardPile();
	void dealCards();
	void coinToss();
	void endRound();


private:
	vector<Player*> m_playerList;
	unsigned m_round_num;
	vector<Card> m_drawPile;
	vector<Card> m_discardPile;
	Player* m_nextPlayer;
	unsigned m_numPlayers;
};

#endif // !ROUND_H