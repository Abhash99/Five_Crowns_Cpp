#include "stdafx.h"
#include "Game.h"

/*****************************************************************************************************************
/* Function Name: startGame
/* Purpose: To start a game (either new game or load previously saved game)
/* Parameters: None	
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			Print the main menu, prompt the user for input and get the choice from user.
			If the user wants to start a new game, we call the startNew function.
			If the user wants to load a saved game, we call the startLoad function.
			Else exit the program. 
/* Assistance Received: None

******************************************************************************************************************/
void Game::startGame()
{
	printMainMenu();
	unsigned input = getMainMenuInput();

	if (input == newGame)
	{
		unsigned ret = startNew();
		if (ret == 0)
		{
			printWinner();
		}
		else
		{
			cout << "Five Crowns Program ended abruptly." << endl;
			exit(1);
		}

	}
	else if (input == loadGame)
	{
		unsigned ret = startLoad();
		if (ret == 0)
		{
			printWinner();
		}
		else
		{
			cerr << "Five Crowns Program ended abruptly." << endl;
			exit(1);
		}
	}
	else
	{
		exit(0);
	}

}

/*****************************************************************************************************************
/* Function Name: printMainMenu
/* Purpose: To print the main menu options for the user
/* Parameters: None
/* Return Value: None
/* Local Variables: None
/* Algorithm: Simply print the menu option in the screen.
/* Assistance Received: None

******************************************************************************************************************/
void Game::printMainMenu()
{
	cout << "================= Welcome to Five Crowns ====================" << endl;
	cout << "Menu Options: " << endl;
	cout << "1. Play Game" << endl;
	cout << "2. Load Previously Saved Game" << endl;
	cout << "3. Quit Game" << endl;
}

/*****************************************************************************************************************
/* Function Name: getMainMenuInpute
/* Purpose: To get the main menu input from the user
/* Parameters: None
/* Return Value: 
			unsigned input - The option that the user chooses is returned. 
/* Local Variables: None
/* Algorithm:
			Prompt the user for input and obtain the input from the user.
			Repeat until the input is between 1 and 3. 
			Return the input.
/* Assistance Received: None

******************************************************************************************************************/
unsigned Game::getMainMenuInput()
{
	unsigned input;
	do
	{
		cout << "Please enter your option (1-3): ";
		cin >> input;
	} while (!(input >= 1 && input <= 3));
	cout << endl;
	return input;
}

/*****************************************************************************************************************
/* Function Name: getPlayers
/* Purpose: 
			To get the number of players, the player names and number to create new players that will be playing the
			game.
/* Parameters: None
/* Return Value: None
/* Local Variables:
				Player* newPlayer - a pointer to the new human/computer object that is newly created.
/* Algorithm:
			Prompt the user to ask the number of players playing the game.
			Update the member variable m_numPlayer based on user input.
			For each player, ask the player's name and ask if they are human.
			If player is human, create a new human object and push it into player list. 
			Else create a new computer object and push it into the player list. 

/* Assistance Received: None

******************************************************************************************************************/
void Game::getPlayers()
{
	// Ask number of players
	unsigned numPlayers;
	do
	{
		cout << "Enter number of players: ";
		cin >> numPlayers;
	} while (!(numPlayers >= 2));
	cout << endl;

	this->m_numPlayers = numPlayers;

	for (unsigned i = 0; i < numPlayers; i++)
	{
		// Ask player's name
		string name;
		cout << "Please enter name of player " << i << ": ";
		cin >> name;

		unsigned playerNumber = i;

		// Ask if player is human 
		char isHuman;
		cout << "Is the player human? (y/n): ";
		cin >> isHuman;

		cout << endl;
		isHuman = tolower(isHuman);
		if (isHuman == 'y')
		{
			Player* newPlayer = new Human(name, playerNumber);
			newPlayer->setIsHuman(true);
			m_playerList.push_back(newPlayer);
		}
		else
		{
			Player* newPlayer = new Computer("Computer", playerNumber);
			newPlayer->setIsHuman(false);
			m_playerList.push_back(newPlayer);
		}
	}
}

/*****************************************************************************************************************
/* Function Name: startNew	
/* Purpose: To start a new game
/* Parameters: None	
/* Return Value:
				Returns 0 if the initiateRound() ended successfully, and 1 if the initiateRound() ended unsuccessfully.
/* Local Variables: 
				Round* newRound - A pointer to a new object of Round Class that is created to initiate the game.
/* Algorithm:
			First, we get the number of players and players' information.
			Based on the data received, we create a new round object with round number 1.
			Then, we initiate the round and get its return value which will ultimately be returned by this function. 
/* Assistance Received: None

******************************************************************************************************************/
unsigned Game::startNew()
{
	getPlayers();
	Round* newRound = new Round(m_playerList, NEW_ROUND_NUM, m_numPlayers);
	unsigned returnVal = newRound->initiateRound();

	return returnVal;

}

/*****************************************************************************************************************
/* Function Name: startLoad
/* Purpose: To load a already saved game (load a round state)
/* Parameters: None
/* Return Value:
			Returns 0 if the round ended successfully, and 1 otherwise.
/* Local Variables:
				vector<Player*> playerList - a temporary vector of Player* to load all the players from the load file.
				vector<Card> drawPile - a temporary vector of cards to load drawpile from the load file. 
				vector<Card> discardPile - a temporary vector of cards to load discardPile from the load file. 
				Player* nextPlayer - a temporary Player* that points to the next player, which has been stored in the
				load file.
				Serializer loadClient - A serializer object that is used to load the round state.
				Round* loadRound - A pointer to a new round object that will be initiated after the round state is
				loaded. 
/* Algorithm:
			We use the load client to get round number, playerlist, draw pile, discard pile and next player.
			Then we set the member variables (playerList and numPlayers) of the game class based on the load data.
			Finally, we initiate the round (based on the load data) and return the return value from the
			initiateRound() funtion. 
/* Assistance Received: None

******************************************************************************************************************/
unsigned Game::startLoad()
{
	unsigned roundNum;
	vector<Player*> playerList;
	vector<Card> drawPile;
	vector<Card> discardPile;
	Player* nextPlayer;

	Serializer loadClient;
	loadClient.loadData(playerList, roundNum, drawPile, discardPile, nextPlayer);

	// After the loading of elements is complete, we need to update the member variables of the game class
	this->m_playerList = playerList;
	this->m_numPlayers = playerList.size();


	Round* loadRound = new Round(playerList, roundNum, playerList.size(), drawPile, discardPile, nextPlayer);
	unsigned returnVal = loadRound->initiateRound();

	return returnVal;
}


/*****************************************************************************************************************
/* Function Name: printWinner
/* Purpose: To print the final winner of the game
/* Parameters: None	
/* Return Value: None	
/* Local Variables:
				Player* winner - a pointer that points to the winner.
/* Algorithm:
			For each player in the playerList, we check the score for each player. 
			The player with the lowest score will be declared as the winner. 
/* Assistance Received: None

******************************************************************************************************************/
void Game::printWinner()
{
	// We set lowest score to a large number so that any score value will be 
	// set to lowest when first encountered. 
	unsigned lowestScore = 1000;
	Player* winner = NULL;
	cout << endl;
	cout << "The Final Scores are: " << endl;
	for (auto player : this->m_playerList)
	{
		cout << player->getName() << ": " << player->getScore() << endl;
		if (player->getScore() < lowestScore)
		{
			lowestScore = player->getScore();
			winner = player;
		}
	}

	cout << "===================================================" << endl;
	cout << "The Winner of the game is " << winner->getName() << ". Congratulations " << winner->getName() << "!" << endl;
	cout << endl;

	cout << "=========================== THANK YOU FOR PLAYING FIVE CROWNS ===============================================" << endl;
	exit(0);
}
