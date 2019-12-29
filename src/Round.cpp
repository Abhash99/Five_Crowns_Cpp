#include "stdafx.h"
#include "Round.h"


/*****************************************************************************************************************
/* Function Name: Round 
/* Purpose: Constructor of Round class
/* Parameters:
			vector<Player*> playerList - Holds the players playing the game
			unsigned round_num - Specifies the round number to start the game from
			unsigned numPlayers - The number of players playing the game (computer & human)
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			Simply initialize the member variables to the parameters. 
/* Assistance Received: None

******************************************************************************************************************/
Round::Round(vector<Player*> playerList, unsigned round_num, unsigned numPlayers)
{
	this->m_playerList = playerList;
	this->m_round_num = round_num;
	this->m_numPlayers = numPlayers;
	initializeDrawPile();
	clearPlayerHands();
	dealCards();
	addTodiscardPile(pickFromDrawPile());
	if (round_num == 1)
	{
		coinToss();
	}
}

/*****************************************************************************************************************
/* Function Name: Round
/* Purpose: Constructor of Round class
/* Parameters:
			vector<Player*> playerList - Holds the players playing the game
			unsigned round_num - Specifies the round number to start the game from
			unsigned numPlayers - The number of players playing the game (computer & human)
			vector<Card> drawPile - DrawPile state for the round
			vector<Card> discardPile - Discard pile state for the round
			Player* nextPlayer - Next player to play the turn 
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			Simply initialize the member variables to the parameters.
/* Assistance Received: None


******************************************************************************************************************/
Round::Round(vector<Player*> playerList, unsigned round_num, unsigned numPlayers, vector<Card> drawPile, vector<Card> discardPile, Player* nextPlayer)
{
	this->m_playerList = playerList;
	this->m_round_num = round_num;
	this->m_numPlayers = numPlayers;
	this->m_drawPile = drawPile;
	this->m_discardPile = discardPile;
	this->m_nextPlayer = nextPlayer;
}

/*****************************************************************************************************************
/* Function Name: initiateRound
/* Purpose: To start a round state
/* Parameters: None
/* Return Value: Returns 0 if the round ended successfully or 1 otherwise. 
/* Local Variables:
				Player* lastPlayer - holds the last player to go out

/* Algorithm:
			We start by setting the wildcards based on the round number. 
			We display the round information, and we keep alternating turns of players. 
			In each turn, the player picks a card, drops a card, tries to make books/runs and go out if possible. 
			The human player also has the ability to save the game or quit the game before each turn.
			If a player goes out, every player plays one last turn, declares their runs/books and get their hands
			evaluated. The player who went out will be the first one to play in the next round.
			Then we end the round where every thing is initialized and we start the next round. 
/* Assistance Received: None

******************************************************************************************************************/
unsigned Round::initiateRound()
{
	while (this->m_round_num <= MAX_ROUND_NUM)
	{
		// Make wildcard
		makeWildcard();

		// Initiate Play function for players unless the round ends.
		Player* lastPlayer = m_nextPlayer;
		// Keep alternating turns until one of the player goes out
		lastPlayer->setGoOutFlag(false);
		while (lastPlayer->wentOut() == false)
		{
			printRoundInfo();
			m_nextPlayer->printMenu();
			unsigned choice = m_nextPlayer->getMenuInput();

			if (choice == 1)
			{
				saveGame();
			}
			else
			{
				m_nextPlayer->play(m_drawPile, m_discardPile);
				unsigned nextPlayerNum = (m_nextPlayer->getPlayerNum() + 1) % m_numPlayers;
				lastPlayer = m_nextPlayer;
				setNextPlayer(m_playerList[nextPlayerNum]);
			}
		}

		// Since a player went out, we need to play last turn for all players, starting from the player who went out.
		while(getNextPlayer() != lastPlayer)
		{
			printRoundInfo();
			m_nextPlayer->lastTurn(m_drawPile, m_discardPile);
			unsigned nextPlayerNum = (m_nextPlayer->getPlayerNum() + 1) % m_numPlayers;
			setNextPlayer(m_playerList[nextPlayerNum]);
		}

		// Clear the flag for the player who went out
		lastPlayer->setGoOutFlag(false);

		// Set the player to go out first as the first player of next round
		setNextPlayer(lastPlayer);
		// End current round (reset piles, hands, and deal cards)
		endRound();
	}

	return 0;
}

/*****************************************************************************************************************
/* Function Name: endRound
/* Purpose: To end the round
/* Parameters: None
/* Return Value: None
/* Local Variables:
/* Algorithm:
			We increment the round number after each round ends.
			We clear the draw and discard piles. 
			We clear player's hands. 
			We initialize drawPile and deal cards. 
			Finally we add the top of draw pile to discard pile. 
			Then the round is started again. 
/* Assistance Received: None

******************************************************************************************************************/
void Round::endRound()
{
	// Increase the round number
	this->m_round_num++;

	// Clear drawpile and discard pile
	m_drawPile.clear();
	m_discardPile.clear();

	// Clear playerHands
	clearPlayerHands();

	// Initialize drawPile
	initializeDrawPile();

	// Deal Cards
	dealCards();

	// Set one card for discardPile
	addTodiscardPile(pickFromDrawPile());

}

/*****************************************************************************************************************
/* Function Name: initializeDrawPile
/* Purpose: To initialize the draw pile after each round.
/* Parameters: None
/* Return Value: None
/* Local Variables:
				Deck d1, d2 - Two deck objects that are created and mixed to form the drawpile. 
				vector<Card> temp1 - stores cards of deck1
				vector<Card> temp2 - stores cards of deck2
				vector<Card> newPile - a temporary vector of cards that holds both the decks, essentially the drawPile
/* Algorithm:
			We create two deck objects and initialize them. We shuffle both the decks and concatenate both of them into
			a vector of cards, which is our draw pile. 
/* Assistance Received: None

******************************************************************************************************************/
void Round::initializeDrawPile()
{
	Deck d1, d2;
	d1.initializeDeck();
	d2.initializeDeck();
	//Make wildcard here
	d1.shuffle();
	d2.shuffle();
	vector<Card> temp1 = d1.getDeck();
	vector<Card> temp2 = d2.getDeck();
	vector<Card> newPile;
	newPile.insert(newPile.begin(), temp1.begin(), temp1.end());
	newPile.insert(newPile.end(), temp2.begin(), temp2.end());
	m_drawPile = newPile;
	makeWildcard();
}

/*****************************************************************************************************************
/* Function Name: printRoundInfo
/* Purpose: To print the key round information (i.e. round num, player details, hands, draw piles and discard piles, etc)
/* Parameters: None
/* Return Value: None
/* Local Variables: None
/* Algorithm: We simply print the key round information to the screen.
/* Assistance Received: None

******************************************************************************************************************/
void Round::printRoundInfo()
{
	cout << endl;
	cout << "Round:" << this->m_round_num << endl;
	for (auto player : m_playerList)
	{
		cout << player->getName() << ":" << endl;
		cout << "\tScore: " << player->getScore() << endl;
		cout << "\t";
		player->getHand().displayHand();
		cout << endl;
		cout << endl;

	}
	printDrawPile();
	printDiscardPile();
	cout << endl;
	cout << "Next Player: " << m_nextPlayer->getName() << endl;
	cout << endl;

}

/*****************************************************************************************************************
/* Function Name: saveGame
/* Purpose: To save the round state
/* Parameters: None
/* Return Value: None
/* Local Variables: 
				Serializer saveClient - a serializer object that saves the round information into a text file. 
/* Algorithm: 
			We use the serialzier object to save the round state into a text file. 
/* Assistance Received: None

******************************************************************************************************************/
void Round::saveGame()
{
	Serializer saveClient;
	saveClient.saveRound(m_playerList, m_round_num, m_drawPile, m_discardPile, m_nextPlayer);
	exit(0);

}

/*****************************************************************************************************************
/* Function Name: makeWildcard
/* Purpose: To set the wildcards based on the round number
/* Parameters: None
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			For each card in the drawPile, if the card's rank equals the current round number + 2, set the card 
			as wildcard. 
/* Assistance Received: None

******************************************************************************************************************/
void Round::makeWildcard()
{
	for (unsigned i = 0; i < m_drawPile.size(); i++)
	{
		// Need to exclude jokers from it. 
		if ((m_drawPile[i].getRank() == (m_round_num + 2)) && (m_drawPile[i].isJoker() == false))
		{
			m_drawPile[i].setWildcard(true);
			m_drawPile[i].setValue(20);
		}
	}
}

/*****************************************************************************************************************
/* Function Name: clearPlayerHands
/* Purpose: To clear hands of the all players playing the game
/* Parameters: None
/* Return Value: None
/* Local Variables: None
/* Algorithm: Simply clear the hands of each player iteratively.
/* Assistance Received: None

******************************************************************************************************************/
void Round::clearPlayerHands()
{
	for (auto player : m_playerList)
	{
		player->clearHand();
	}
}

/*****************************************************************************************************************
/* Function Name: printDrawPile
/* Purpose: To print the cards in drawPile
/* Parameters: None
/* Return Value: None
/* Local Variables:
				vector<card> copyDrawPile - copy of the drawpile

/* Algorithm:
			For each card in the drawPile, we convert them to string and print the cardstring.
/* Assistance Received: None

******************************************************************************************************************/
void Round::printDrawPile()
{
	cout << endl;
	cout << "Draw Pile: ";
	vector<Card> copyDrawPile = getDrawPile();
	// Reverse is used becuase the implementation of top of the piles was designed in an opposite way
	reverse(copyDrawPile.begin(), copyDrawPile.end());
	for (vector<Card>::iterator it = copyDrawPile.begin(); it != copyDrawPile.end(); it++)
	{
		cout << (*it).toString() + " ";
	}
	cout << endl;
}

/*****************************************************************************************************************
/* Function Name: printDiscardPile
/* Purpose: To print discardPile
/* Parameters: None
/* Return Value: None
/* Local Variables: vector<Card> copyDiscardPile - holds the copy of the discardPile
/* Algorithm:
			For each card in the discardPile, we convert them to string and print the cardstring.
/* Assistance Received: None

******************************************************************************************************************/
void Round::printDiscardPile()
{
	cout << endl;
	cout << "Discard Pile: ";
	vector<Card> copyDiscardPile = getDiscardPile();
	// Reverse is used becuase the implementation of top of the piles was designed in an opposite way
	reverse(copyDiscardPile.begin(), copyDiscardPile.end());

	for (vector<Card>::iterator it = copyDiscardPile.begin(); it != copyDiscardPile.end(); it++ )
	{
		cout << (*it).toString() + " ";
	}
	cout << endl;
}

/*****************************************************************************************************************
/* Function Name: dealCards
/* Purpose: To deal cards to each player based on the round number
/* Parameters: None
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			For each player in playerList, add card to player's hand based on the round number. 
/* Assistance Received: None

******************************************************************************************************************/
void Round::dealCards()
{
	for (unsigned i = 0; i < m_round_num + 2; i++)
	{
		for (auto player : m_playerList)
		{
			player->addCard(pickFromDrawPile());
		}
	}
}

/*****************************************************************************************************************
/* Function Name: coinToss
/* Purpose: To hold the coin toss to decide the player to play first
/* Parameters: None
/* Return Value: none
/* Local Variables: None
/* Algorithm:
			We ask the human player to call for the coin toss. If the human wins, set human as next player. Else, 
			computer is the next player. 
/* Assistance Received: None

******************************************************************************************************************/
void Round::coinToss()
{
	char choice;
	unsigned flip;
	srand(unsigned(time));
	cout << "We will toss a coin to decide who goes first." << endl;
	for (auto player : m_playerList)
	{
		if (player->isHuman() == true)
		{
			player->askForCall(choice);
			flip = rand() % 2;

			switch (flip)
			{
			case 0:
				if (choice == 'h')
				{
					cout << player->getName() + " won the toss. " + player->getName() + " goes first." << endl;
					setNextPlayer(player);
				}
				else
				{
					unsigned nextPlayerNum = (player->getPlayerNum() + 1) % m_numPlayers;
					setNextPlayer(m_playerList[nextPlayerNum]);
					cout << m_playerList[nextPlayerNum]->getName() + " won the toss. " + m_playerList[nextPlayerNum]->getName() + " goes first." << endl;
				}
				break;
			case 1:
				if (choice == 't')
				{
					cout << player->getName() + " won the toss. " + player->getName() + " goes first.";
					setNextPlayer(player);
				}
				else
				{
					unsigned nextPlayerNum = (player->getPlayerNum() + 1) % m_numPlayers;
					setNextPlayer(m_playerList[nextPlayerNum]);
					cout << m_playerList[nextPlayerNum]->getName() + " won the toss. " + m_playerList[nextPlayerNum]->getName() + " goes first." << endl;

				}
				break;
			}
			break;
		}
	}
}


/*****************************************************************************************************************
/* Function Name: addTodiscardPile
/* Purpose: To add a particular card to discard pile
/* Parameters: 
			Card c - The card that is to be added to the discard pile
/* Return Value: None
/* Local Variables: None
/* Algorithm: Simply add the card c to the discard pile using push_back function of vector. 
/* Assistance Received: None

******************************************************************************************************************/
void Round::addTodiscardPile(Card c)
{
	m_discardPile.push_back(c);
}

/*****************************************************************************************************************
/* Function Name: pickFromDrawPile
/* Purpose: To pick the top card from draw pile
/* Parameters: None
/* Return Value: Card topCard - the top card of draw pile
/* Local Variables:
				Card topCard - this card will hold the copy of the top card of the draw pile.
/* Algorithm: 
			Simply save the copy of the top of draw pile in a card variable, pop back the top of the pile and
			return the saved topCard.
/* Assistance Received: None

******************************************************************************************************************/
Card Round::pickFromDrawPile()
{
	Card topCard = m_drawPile.back();
	m_drawPile.pop_back();
	return topCard;
}

/*****************************************************************************************************************
/* Function Name: pickFromDrawPile
/* Purpose: To pick the top card from draw pile
/* Parameters: None
/* Return Value: Card topCard - the top card of draw pile
/* Local Variables:
			Card topCard - this card will hold the copy of the top card of the draw pile.
/* Algorithm:
			Simply save the copy of the top of discard pile in a card variable, pop back the top of the pile and
			return the saved topCard.
/* Assistance Received: None

******************************************************************************************************************/
Card Round::pickFromDiscardPile()
{
	auto topCard = m_discardPile.back();
	m_discardPile.pop_back();
	return topCard;
}

