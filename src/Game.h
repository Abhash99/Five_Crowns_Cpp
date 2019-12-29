/**/
/*********************************************************************************
** Name: Abhash Panta
** Project: Five Crowns (C++)
** Class: CMPS 366-01
** Date: 10/01/2019
**********************************************************************************/
/**/

#pragma once
#include "Round.h"
#include "Player.h"
#include "Human.h"
#include "Computer.h"
#include "Serializer.h"


class Game
{
public:
	//Symbolic Constants
	unsigned MAX_ROUNDS = 11;
	unsigned NEW_ROUND_NUM = 1;

	// Public Enumerators
	enum MainMenu
	{
		newGame = 1,
		loadGame,
		quitGame
	};

	// Constructor and Destructor
	Game()
	{
		this->m_playerList.clear();
		this->m_numPlayers = 0;
	}

	~Game(){}

	//Utility Functions
	void startGame();
	void printMainMenu();
	unsigned getMainMenuInput();
	void getPlayers();
	unsigned startNew();
	unsigned startLoad();
	void printWinner();

private:
	vector<Player*> m_playerList;
	unsigned m_numPlayers;
};

