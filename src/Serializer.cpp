#include "stdafx.h"
#include "Serializer.h"

/*****************************************************************************************************************
/* Function Name: Serializer 
/* Purpose: Constructor of serializer class
/* Parameters: None
/* Return Value: None
/* Local Variables: None
/* Algorithm: Initialize the member variables.
/* Assistance Received: None

******************************************************************************************************************/
Serializer::Serializer()
{
	this->m_labelMap.clear();
	this->m_statements.clear();
}


/*****************************************************************************************************************
/* Function Name: saveRound
/* Purpose: To save the round state into a text file
/* Parameters: 
			vector<Player*> playerList - holds the players playing the game
			unsigned roundNum - holds the current round number
			vector<Card> drawPile - hold the draw pile
			vector<Card> discardPile - holds discard pile
			Player* nextPlayer - holds next player
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			Ask user for the file name to save in. Create the new save file.
			Using filestream, write all the key round data to the text file. 
/* Assistance Received: None

******************************************************************************************************************/
void Serializer::saveRound(vector<Player*> playerList, unsigned roundNum, vector<Card> drawPile, vector<Card> discardPile, Player* nextPlayer)
{
	string filename;
	cout << "Please enter the filename for the save file: " << endl;
	cin >> filename;

	ofstream saveFile;
	saveFile.open(filename);
	if (!saveFile)
	{
		cerr << "Cannot create save file. There was an error. Exiting program" << endl;
		exit(1);
	}

	saveFile << "Round:" << roundNum << endl;
	for (auto player : playerList)
	{
		saveFile << endl;
		if (player->isHuman())
		{
			saveFile << "Human:" << endl;
		}
		else
		{
			saveFile << "Computer:" << endl;
		}
		saveFile << "\tScore: " << player->getScore() << endl;
		saveFile << "\tHand: ";
		for (auto card : player->getHand().getHand())
		{
			saveFile << card.toString() + " ";
		}
		saveFile << endl;
	}

	saveFile << "Draw Pile: ";
	for (auto card : drawPile)
	{
		saveFile << card.toString() + " ";
	}
	saveFile << endl;

	saveFile << "Discard Pile: ";
	for (auto card : discardPile)
	{
		saveFile << card.toString() + " ";
	}
	saveFile << endl;

	saveFile << "Next Player: ";
	if (nextPlayer->isHuman())
	{
		saveFile << "Human";
	}
	else
	{
		saveFile << "Computer";
	}
	saveFile << endl;

	saveFile.close();

	return;
}

/*****************************************************************************************************************
/* Function Name: loadData
/* Purpose: To load the round data from a text file
/* Parameters: 
				playerList - list of players - pass by reference to receive value
				roundNum - current round number
				drawPile - current state of draw pile
				discardPile - current state of discard pile
				nextPlayer - next Player to play the turn
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			Simply record statements from the save file and obtain each key data using respective getter functions.
/* Assistance Received: None

******************************************************************************************************************/
void Serializer::loadData(vector<Player*> &playerList, unsigned &roundNum, vector<Card> &drawPile, vector<Card> &discardPile, Player* &nextPlayer)
{
	recordStatements();
	playerList = getPlayerList();
	roundNum = getRoundNum();
	drawPile = getDrawPile();
	discardPile = getDiscardPile();
	nextPlayer = getNextPlayer(playerList);
}


/*****************************************************************************************************************
/* Function Name: recordStatements
/* Purpose: To record statements in the save file as a vector
/* Parameters: None
/* Return Value: None
/* Local Variables: None
/* Algorithm: Use filestream object to record the statements.
/* Assistance Received: None

******************************************************************************************************************/
void Serializer::recordStatements()
{
	string filename;
	ifstream loadFile;

	cout << endl;
	cout << "Please enter the name of the load file with extension: " << endl;
	cin >> filename;

	loadFile.open(filename, ios::in);

	if (!loadFile)
	{
		cerr << "Could not open file!";
		exit(1);
	}

	// Variable Declaration
	string line;
	size_t index;
	string label;
	string stat;

	int counter = 0;
	// Read the file, one line at a time
	while (getline(loadFile, line))
	{
		// If the line isn't empty
		if (line != "")
		{
			// Remove leading whitespace from the line
			index = line.find_first_not_of(" \t");
			line = line.substr(index);

			string delimeter = ":";

			string label = line.substr(0, line.find(delimeter));
			index = label.find_first_not_of(" \t");
			label = label.substr(index);

			string stat = line.substr((line.find(delimeter)) + 1, line.length());

			m_labelMap.insert(pair<int, string>(counter, label));
			counter++;
			m_statements.push_back(stat);
		}
	}

	return;
}

/*****************************************************************************************************************
/* Function Name: getRoundNum
/* Purpose: To obtain the round number from the save file
/* Parameters: None
/* Return Value: returns the round number
/* Local Variables: None
/* Algorithm:
			Iterate throught the statements vector and get the corresponding value next to the "Round" string
			Return the value. 
/* Assistance Received: None

******************************************************************************************************************/
unsigned Serializer::getRoundNum()
{
	map<int, string>::iterator it;
	it = m_labelMap.begin();
	if (it->second == "Round")
	{
		int index = it->first;
		string sNum = m_statements[index];
		int number = 0;
		number = stoi(sNum);
		unsigned roundNum = (unsigned)number;
		return roundNum;
	}
	else
	{
		cerr << "Invalid format of Load File. Exiting Program!";
		return (EXIT_FAILURE);
	}

}

/*****************************************************************************************************************
/* Function Name: getPlayerList
/* Purpose: To obtain playerList from the save file
/* Parameters: None
/* Return Value: None
/* Local Variables: 
				vector<Player*> playerList -the vector that hols the players playing the game
/* Algorithm:
			We iterate throught the statement vector to get the element that ends with a ':'. That marks a new player so
			create a new player based on their name and add them to our playerList. 
/* Assistance Received: None

******************************************************************************************************************/
vector<Player*> Serializer::getPlayerList()
{
	vector<Player*> playerList;
	Player* player = NULL;
	map<int, string>::iterator it;
	int index = 0;
	unsigned playerNum = 0;
	for (it = m_labelMap.begin(); it != m_labelMap.end(); it++)
	{
		
		if (it->second == "Draw Pile")
		{
			break;
		}

		if (it != m_labelMap.begin())
		{
			index = it->first;

			if (m_statements[index] == "")
			{
				unsigned score;
				string playerName = it->second;
				Hand playerHand;
				playerName = convertToLower(playerName);
				it++;
				if (it->second == "Score")
				{
					index = it->first;
					int scoreVal = stoi(m_statements[index]);
					score = unsigned(scoreVal);
					it++;

					if (it->second == "Hand")
					{
						index = it->first;
						string str = m_statements[index];
						vector<Card> pile = convertToCard(str);
						playerHand.setHand(pile);
						playerHand.setHandSize(pile.size());
					}
					else
					{
						cerr << "Invalid format of Load File. Exiting Program!" << endl;
						exit(1);
					}
				}
				else
				{
					cerr << "Invalid format of Load file. Exiting Program!" << endl;
					exit(1);
				}

				if (playerName == "human")
				{
					player = new Human(playerName, playerNum);
					

				}
				else if (playerName == "computer")
				{
					player = new Computer(playerName, playerNum);
				}

				playerNum++;
				player->setHand(playerHand);
				player->setScore(score);
				player->setGoOutFlag(false);
				playerList.push_back(player);
			}
		}
	}
	return playerList;

}

/*****************************************************************************************************************
/* Function Name: getDrawPile
/* Purpose: To read the drawpile from the save file
/* Parameters: None
/* Return Value:
				vector<Card> copy - reversed copy of the card pile that is read.
/* Local Variables:
				vector<Card> cardPile - that holds the card pile read from the stirng
/* Algorithm: 
			We iterate to the statement with "draw pile:" and read the rest of the string delimeted by space one at
			a time.

/* Assistance Received: None

******************************************************************************************************************/
vector<Card> Serializer::getDrawPile()
{
	vector<Card> cardPile;
	cardPile.clear();
	map<int, string>::iterator it;
	for (it = m_labelMap.begin(); it != m_labelMap.end(); it++)
	{
		if (it->second == "Draw Pile")
		{
			int index = it->first;
			cardPile = convertToCard(m_statements[index]);
			break;
		}
	}
	
	// Had to reverse for serialization
	vector<Card> copy = cardPile;
	reverse(copy.begin(), copy.end());
	return copy;
}

/*****************************************************************************************************************
/* Function Name: getDiscardPile
/* Purpose: To read the discardpile from the save file
/* Parameters: None
/* Return Value:
			vector<Card> copy - reversed copy of the card pile that is read.
/* Local Variables:
			vector<Card> cardPile - that holds the card pile read from the stirng
/* Algorithm:
			We iterate to the statement with "discard pile:" and read the rest of the string delimeted by space one at
			a time.

/* Assistance Received: None

******************************************************************************************************************/
vector<Card> Serializer::getDiscardPile()
{
	vector<Card> cardPile;
	cardPile.clear();
	map<int, string>::iterator it;
	for (it = m_labelMap.begin(); it != m_labelMap.end(); it++)
	{
		if (it->second == "Discard Pile")
		{
			int index = it->first;
			cardPile = convertToCard(m_statements[index]);
			break;
		}
	}

	// Had to reverse for serialization
	vector<Card> copy = cardPile;
	reverse(copy.begin(), copy.end());
	return copy;
}

/*****************************************************************************************************************
/* Function Name: getNextPlayer
/* Purpose: To read the next player who will play the turn from the save file
/* Parameters: vector<Player*> playerList
/* Return Value:
			Player* nextPlayer - the player who will play the next turn
/* Local Variables:
			Player* nextPlayer - will point to the next player
/* Algorithm: Read the name next to the "Next Player:" label in the save file and set that player as the next player.
/* Assistance Received: None

******************************************************************************************************************/
Player* Serializer::getNextPlayer(vector<Player*> playerList)
{
	Player* nextPlayer = NULL;
	map<int, string>::iterator it;
	for (it = m_labelMap.begin(); it != m_labelMap.end(); it++)
	{
		if (it->second == "Next Player")
		{
			int index = it->first;
			string playerName = m_statements[index];
			index = playerName.find_first_not_of(" ");
			playerName = playerName.substr(index);
			playerName = convertToLower(playerName);

			for (unsigned i = 0; i < playerList.size(); i++)
			{
				if (playerList[i]->getName() == playerName)
				{
					nextPlayer = playerList[i];
					break;
				}
			}
			break;
		}
	}

	return nextPlayer;
}


/*****************************************************************************************************************
/* Function Name: convertToCard
/* Purpose: To convert a cardstring to card
/* Parameters: string pileString - the string representation of the pile
/* Return Value: vector<Card> cardPile - a vector of cards created based on the string
/* Local Variables:
				vector<Card> cardPile - that holds the cards read from the string
/* Algorithm: 
			We use the convertToCardString function to get the vector of string representing each card and then
			create a card object from each element in the vector.

/* Assistance Received: None

******************************************************************************************************************/
vector<Card> Serializer::convertToCard(string pileString)
{
	vector<string> cardString = convertToCardString(pileString);
	vector<Card> cardPile = getCardVector(cardString);
	return cardPile;
}

/*****************************************************************************************************************
/* Function Name: getCardVector
/* Purpose: To get the vector of cards from a vector of card strings.
/* Parameters: vector<string> cardStringPile - holds all the cardStrings of cards in a pile
/* Return Value: 
				vector<Card> cardPile - the card pile that is created from respective cardstring in the vector of
				strings. 
/* Local Variables: vector<Card> cardPile - holds the cards created from cardstrings
/* Algorithm: Simply create a new card from each cardstring in the vector of strings and push it into the vector of 
				cards. 
/* Assistance Received: None

******************************************************************************************************************/
vector<Card> Serializer::getCardVector(vector<string> cardStringPile)
{
	vector<Card> cardPile;
	for (unsigned i = 0; i < cardStringPile.size(); i++)
	{
		int index = cardStringPile[i].find_first_not_of(" ");
		cardStringPile[i].substr(index);
		Card newCard(cardStringPile[i]);
		cardPile.push_back(newCard);
	}
	return cardPile;
}

/*****************************************************************************************************************
/* Function Name: converToCardString
/* Purpose: To convert a string of multiple cardstrings to multiple strings representing each card.
/* Parameters: string pileString - the string of multiple cardstringss
/* Return Value: vector<string> cardString - a vector that contains string representation of all the cards in the
				pile. 
/* Local Variables: None
/* Algorithm: We delimit the cardstring with the delimiter of space character and store each of them into a vector.
/* Assistance Received: None

******************************************************************************************************************/
vector<string> Serializer::convertToCardString(string pileString)
{
	int index = pileString.find_first_not_of(" ");
	pileString = pileString.substr(index);

	vector<string> cardString;
	char delimeter = ' ';
	string token;
	istringstream parser(pileString);
	while (getline(parser, token, delimeter))
	{
		cardString.push_back(token);
	}

	return cardString;
}

/*****************************************************************************************************************
/* Function Name: convertToLower
/* Purpose: To convert string to lowercase
/* Parameters:
			string str - the string to convert
/* Return Value: None
/* Local Variables: None
/* Algorithm: Uses the transform function to convert each character in the string to lower case. 
/* Assistance Received: None

******************************************************************************************************************/
string Serializer::convertToLower(string &str)
{
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}