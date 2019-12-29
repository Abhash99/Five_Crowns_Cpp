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
#include "Computer.h"
#include "Human.h"


class Serializer
{
public:
	// Default Constructor
	Serializer();

	// Destructor
	~Serializer(){}

	// Utility Dunctions
	void saveRound(vector<Player*> playerList, unsigned roundNum, vector<Card> drawPile, vector<Card> discardPile, Player* nextPlayer);

	
	void loadData(vector<Player*> &playerList, unsigned &roundNum, vector<Card> &drawPile, vector<Card> &discardPile, Player* &nextPlayer);

	// Utility functions
	void recordStatements();
	unsigned getRoundNum();
	vector<Player*> getPlayerList();
	vector<Card> getDrawPile();
	vector<Card> getDiscardPile();
	Player* getNextPlayer(vector<Player*> playerList);

	vector<string> Serializer::convertToCardString(string pileString);
	vector<Card> convertToCard(string pileString);
	vector<Card> getCardVector(vector<string> cardStringPile);
	string convertToLower(string &str);

private:
	map <int, string> m_labelMap;
	vector<string> m_statements;

};

