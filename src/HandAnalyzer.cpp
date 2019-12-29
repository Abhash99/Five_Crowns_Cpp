#include "stdafx.h"
#include "HandAnalyzer.h"

/*****************************************************************************************************************
/* Function Name: HandAnalyzer	
/* Purpose: 
			Default Constructor of HandAnalyzer class that creates an object and initializes all the member
			variables.
/* Parameters: None	
/* Return Value: None	
/* Local Variables: None
/* Algorithm: Initialize all the member variables
/* Assistance Received: None

******************************************************************************************************************/
HandAnalyzer::HandAnalyzer()
{
	this->m_table[Card::M_NUM_SUITS][Card::M_NUM_RANKS - 2] = { 0 };
	this->m_bestCombination.clear();
	this->m_remainingCards.clear();
	this->m_wildCardStack.clear();
}

// Utility Functions
/*****************************************************************************************************************
/* Function Name: loadHandstoTable
/* Purpose: To plot the table based on the cards in hand.
/* Parameters:
			Hand playerHand - the hand that is to be analyzed
/* Return Value: None
/* Local Variables:
				vector<Card> cardsInHand - vector of all cards in hand
/* Algorithm:
			For all cards in hand, if it is not joker or a wildcard, go to the appropriate position in the table
			and increment the value by 1 (E.g. for 5H - We go to row position = 2(suit value for heart) - 1 = 1
			and column position = 5 (rank of the card) - 3 = 2. So, we increment index [1][2] by 1).
/* Assistance Received: None

******************************************************************************************************************/
void HandAnalyzer::loadHandsToTable(Hand playerHand)
{
	vector<Card> cardsInHand = playerHand.getHand();
	for (unsigned i = 0; i < cardsInHand.size(); i++)
	{
		if (!(cardsInHand[i].isJoker() || cardsInHand[i].isWildcard()))
		{
			m_table[(cardsInHand[i].getSuit()) - 1][(cardsInHand[i].getRank()) - 3]++;
		}
	}
	return;
}


/*****************************************************************************************************************
/* Function Name: loadWildCards
/* Purpose: To load the wildcard and jokers in hand to the wildcardstack if present. 
/* Parameters:
			Hand playerHand - the player hand to be analyzed
/* Return Value: None	
/* Local Variables:
				vector<Card> cardsInHand - temporary vector of cards to hold the cards in hand.
/* Algorithm:
				Make a copy of the vector of cards. For each card in the vector of cards, if card is a joker or
				card is a wildcard, push it into the wildcard stack. 
/* Assistance Received: None

******************************************************************************************************************/
// Loads the wildcards and jokers in the wildcardstack if they are present.
void HandAnalyzer::loadWildCards(Hand playerHand)
{
	vector<Card> cardsInHand = playerHand.getHand();
	for (auto card : cardsInHand)
	{
		if (card.isJoker() || card.isWildcard())
		{
			m_wildCardStack.push_back(card);
		}
	}
	return;
}

/*****************************************************************************************************************
/* Function Name: analyzeHand
/* Purpose: Serves as interface function that takes player's hand, does all the analysis and updates the best
			combinations and remaining cards based on the analysis. 
/* Parameters:
			Hand playerHand - The hand that is to be analyzed. 
/* Return Value: None
/* Local Variables:
				unsigned table - a copy of the member variable m_table
				vector<Combination*> allCombiantion - holds all the possible combinations of runs/books and half-runs/half-books
				vector<Card> wildCardStack - copy of the member variable m_wildCardStack
				vector<Combiantion*> helper - A helper function that will be used to store possible combinations.
				vector<vector<Combination*>> combinationCollection - A combination of combinations of runs/books.
				vector<Combination*> bestCombination - Holds the best possible combination that can be obtained from
				current hand.
				vector<Card> remCards - Holds the remaining cards after the best combinations are removed from the current
				hand. 
/* Algorithm:
			First it loads all the cards in the hand to the frequency table. It also loads the wildcards in the hand
			to the wildcard stack. 
			We obtain all the possible combinations as a vector of Combination*. 
			Then, we pass on the vector of all possible combinations to a recursive function that gives us a list of
			possible independent combinations.
			From there, we take the vector of combinations that yields us the lowest remaining cards, which is out best
			combination. We also extract the remaining cards and update the values of member variables bestCombination
			and remainingCards.

/* Assistance Received: None

******************************************************************************************************************/
// Interface Function
void HandAnalyzer::analyzeHand(Hand playerHand)
{
	// Load cards to table and wildcard stack
	loadHandsToTable(playerHand);
	loadWildCards(playerHand);

	// Make a copy of the frequency table
	unsigned table[Card::M_NUM_SUITS][Card::M_NUM_RANKS - 2];
	copyTable(m_table, table);


	// Get all Combinations Initially
	vector<Combination*> allCombinations = extractCardObjects(table, m_wildCardStack);
	vector<Card> wildCardStack = m_wildCardStack;

	vector<Combination*> helper;
	vector<vector<Combination*>> combinationCollection;

	getAllCombinations(playerHand, allCombinations, table, wildCardStack, helper, combinationCollection);
	vector<Combination*> bestCombination = getBestCombination(playerHand, combinationCollection);

	unsigned bestRemainingCards = getRemainingCardsNum(playerHand, bestCombination);
	vector<Card> remCards = getRemainingVector(playerHand, bestCombination);

	// Set Best Combination and Remaining Cards
	setCombination(bestCombination);
	setRemainingCards(remCards);
}


/*****************************************************************************************************************
/* Function Name: getRemainingVector
/* Purpose: To obtain a list of remaining cards after extracting best combination from the hand
/* Parameters:
				Hand playerHand - the player's hand that is being analyzed.
				vector<Combination*> bestCombination - the best combination of cards (runs/books) that is possible
				from the player's current hand. 
/* Return Value: 
				vector<Card> remCards - A vector that contains the remaining cards in hand after setting aside the
				runs/books. 
/* Local Variables: 
					vector<Card> remCards - temporary vector of cards to hold remaining cards
					vector<Card> usedCards - temporary vector of cards to hold cards that have been used in a
					combination.
/* Algorithm:
			We loop throught the combinations and push all the cards in the combinations to usedCards vector. 
			Then in player's hand, if the card is not in the used card stack, we push it into the remCards vector.
			At the end, the remCards vector will contain all the cards that are not in a run/book. 
/* Assistance Received: None

******************************************************************************************************************/
vector<Card> HandAnalyzer::getRemainingVector(Hand playerHand, vector<Combination*> bestCombination)
{
	vector<Card> remCards;
	vector<Card> usedCards;
	// Push all the cards in the combination to a used card stack. 
	for (unsigned i = 0; i < bestCombination.size(); i++)
	{
		for (auto card : bestCombination[i]->getPile())
		{
			usedCards.push_back(card);
		}
	}

	// For each card in player's hand, if the card is not present in the used card
	// stack, push it into the remaining card stack. 
	for (auto card : playerHand.getHand())
	{
		if (find(usedCards.begin(), usedCards.end(), card) == usedCards.end())
		{
			// The card is not present. Push it into the remCards vector.
			remCards.push_back(card);
		}
	}

	return remCards;

}

/*****************************************************************************************************************
/* Function Name: getBestCombination
/* Purpose: To generate best combination from a list of combinations
/* Parameters:
			Hand playerhand - Player's hand that is to be analyzed.
			vector<vector<Combination*>> combinationCollection - Collection of all the independent possible combinations. 
/* Return Value:
			vector<Combination> bestCombination - returns the best combination (the combination that will yield in the
			lease number of remaining cards. 
/* Local Variables:
			vector<Combination*< bestCombination - temporary vector to hold the best combination. 
/* Algorithm:
			We simply iterate through the list of all possible independent combinations, find the one that yields the least
			number of remaining cards and set that as the best combination. 
/* Assistance Received: None

******************************************************************************************************************/
vector<Combination*> HandAnalyzer::getBestCombination(Hand playerHand, vector<vector<Combination*>> combinationCollection)
{
	vector<Combination*> bestCombination;
	unsigned bestRemainingCards = playerHand.getHand().size();

	for (auto newCombinationSet : combinationCollection)
	{
		unsigned remCards = getRemainingCardsNum(playerHand, newCombinationSet);
		if (remCards < bestRemainingCards)
		{
			bestRemainingCards = remCards;
			bestCombination = newCombinationSet;
		}
	}

	return bestCombination;
}


/*****************************************************************************************************************
/* Function Name: getRemainingCardsNum	
/* Purpose: To get the number of cards remaining after a particular combination has been set aside from player's hand. 
/* Parameters:
			Hand playerHand - The player's hand that is to be analyzed. 
			vector<Combination*> newCombination - The combination that is to be set aside from the player's hand. 
/* Return Value:
			Returns the number of cards remaining in the hand after setting aside runs/books. 
/* Local Variables: None
/* Algorithm:
			We simply get the number of cards in the combinations and the number of cards in player's hand.
			We take the difference of the two and return it as the number of remaining cards. 
/* Assistance Received: None

******************************************************************************************************************/
unsigned HandAnalyzer::getRemainingCardsNum(Hand playerHand, vector<Combination*> newCombination)
{
	unsigned counter = 0;
	if (!newCombination.empty())
	{
		for (auto combination : newCombination)
		{
			unsigned numCards = combination->getPile().size();
			counter = counter + numCards;
		}
	}
	unsigned cardsInHand = playerHand.getHand().size();
	unsigned remainingCards = cardsInHand - counter;

	return remainingCards;
}


/*****************************************************************************************************************
/* Function Name: getAllCombinations
/* Purpose: To generate all the possible independent combinations of run/books from a hand. 
/* Parameters:
			Hand playerHand - the player's hand which is to be analyzed. 
			vector<Combination*> allCombinations - the list of all possible combinations of runs/books. (not independent)
			table[][] - the frequency table representing the cards in player's hand. 
			wildCardStack - the list of wildcards in player's hand. 
			vector<Combination*> helper - A helper vector that stores the sequences of combination from the root to a particular leaf.
			vector<vector<Combination*>> combinationCollection - A list containing all the possible independent combination of runs/books.
/* Return Value: None
/* Local Variables: 
				copy_table[][] - Copy of the frequency table
				vector<Card> copyWildCardStack - Copy of the wildcard stack. 

/* Algorithm:
			From the list of all combination possible, we take each combination and recursively search for all the possible combinations that can
			go with it (no overlaps). When we reach from the root to a leaf, we save the combination to combinationCollection and reset the helper
			function for new sequence of combinations. We do this recursively until all the possible combination are exhausted. 
/* Assistance Received: None

******************************************************************************************************************/
void HandAnalyzer::getAllCombinations(Hand playerHand, vector<Combination*> allCombinations, const unsigned table[][Card::M_NUM_RANKS - 2], vector<Card> &wildCardStack, vector<Combination*> &helper, vector<vector<Combination*>> &combinationCollection)
{
	// Base case. Return when the combination vector is empty
	if (allCombinations.empty())
	{
		combinationCollection.push_back(helper);
		return;
	}
	else
	{
		for (unsigned i = 0; i < allCombinations.size(); i++)
		{
			unsigned copy_table[Card::M_NUM_SUITS][Card::M_NUM_RANKS - 2];
			copyTable(table, copy_table);
			vector<Card> copyWildCardStack = wildCardStack;

			// If it is an incomplete run/book (half-run or book)
			// Erase from table and update wildcardState if necessary
			if (allCombinations[i]->isIncomplete())
			{
				for (auto card : allCombinations[i]->getPile())
				{
					if (card.isJoker() || card.isWildcard())
					{
						if (copyWildCardStack.size() > 0)
						{
							copyWildCardStack.pop_back();
						}
					}
				}
			}
			// Erase the current combination from the table
			eraseFromTable(allCombinations[i], copy_table);

			// Save the combination to the helperPile;
			helper.push_back(allCombinations[i]);
			vector<Combination*> newCombination = extractCardObjects(copy_table, copyWildCardStack);
			getAllCombinations(playerHand, newCombination, copy_table, copyWildCardStack, helper, combinationCollection);
			if (!helper.empty())
			{
				helper.pop_back();
			}
		}
	}

}


/*****************************************************************************************************************
/* Function Name: eraseFromTable	
/* Purpose: To remove the card frequency from the table 
/* Parameters:
			Combination* combination - the combination of cards (either runs/books) that needs to be erased from
			the table.
			table[][] - The frequency table from where the combination is to be removed.
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			We go to the appropriate index for each card in the combination and decrement it by 1.
			Note: We decrement it by 1 because there is a possibility of having two cards of the same rank and suit. 
/* Assistance Received: None

******************************************************************************************************************/
void HandAnalyzer::eraseFromTable(Combination* combination, unsigned table[][Card::M_NUM_RANKS - 2])
{
	for (auto card : combination->getPile())
	{
		if (!(card.isWildcard() || card.isJoker()))
		{
			unsigned suit = card.getSuit();
			unsigned rank = card.getRank();
			if (table[suit - 1][rank - 3] != 0)
			{
				table[suit - 1][rank - 3]--;
			}
		}
	}

}

/*****************************************************************************************************************
/* Function Name: extractCardObjects
/* Purpose: To obtain potential runs/books or half-run/half-books from the current hand
/* Parameters:
			table[][] - The frequency table from where we can extract the combinations.
			wildCardStack - The list of wildcards/jokers that could be used to make a run/book
			
/* Return Value:
				vector<Combination*> allCombinations - Returns a vector containing all the potential runs/books in
				a player's hand. 
/* Local Variables:
				vector<Combination*> runList - A list of all the runs
				vector<Combination*> bookList - A list of all the books
				vector<Combination*> halfRuns - A list of all the half-runs (uses jokers)
				vector<Combination*> halfBooks - A list of all the half-books (uses jokers)
				vector<Combination*> combinations - A composite list containing all of the above
/* Algorithm:
			We obtain vectors containing runs, books, half-runs and half-books. 
			We concatenate the all these vectors to one vector and return that vector. 
/* Assistance Received: None

******************************************************************************************************************/
// Reads runs and books from the table (permits overlaps) and returns a vector of CardObjects
vector<Combination*> HandAnalyzer::extractCardObjects(const unsigned table[][Card::M_NUM_RANKS - 2], vector<Card> wildCardStack)
{
	vector<Combination*> combinations;

	// Extract Combinations
	vector<Combination*> runList = extractRuns(table, wildCardStack);
	vector<Combination*> bookList = extractBooks(table, wildCardStack);
	vector<Combination*> halfRuns = extractHalfRuns(table, wildCardStack);
	vector<Combination*> halfBooks = extractHalfBooks(table, wildCardStack);

	// Concatenate all these combinations into one particular vector
	combinations.insert(combinations.end(), runList.begin(), runList.end());
	combinations.insert(combinations.end(), bookList.begin(), bookList.end());
	combinations.insert(combinations.end(), halfRuns.begin(), halfRuns.end());
	combinations.insert(combinations.end(), halfBooks.begin(), halfBooks.end());

	return combinations;
}

/*****************************************************************************************************************
/* Function Name: considerPotentials
/* Purpose: To obtain the potential runs/books - cards that are not complete runs/books but have the potential to 
			be a complete book with one additional card. 
/* Parameters:
			Hand remainingCards - a hand of remainingCards that is to be analyzed for potential runs/books. 
/* Return Value: 
			vector<Combination*> potentialCombs - a vector of potential combination
/* Local Variables: None
/* Algorithm:
			We simply load the hand to table, and extract potential combinations using the funtion extractPotentials().
/* Assistance Received: None

******************************************************************************************************************/
vector<Combination*> HandAnalyzer::considerPotentials(Hand remainingCards)
{
	loadHandsToTable(remainingCards);
	vector<Combination*> potentialCombs = extractPotentials(this->m_table);
	return potentialCombs;
}

/*****************************************************************************************************************
/* Function Name: extractPotentials
/* Purpose:	To return the possible combination that could be a run/book with additional card that matches. 
/* Parameters:
			table[][] - the frequency table representing the cards that are to be considered. s
/* Return Value: vector<Combination*> potentialCombs - a vector of potential combination
/* Local Variables: 
				vector<Combination*> potentialRuns - A list of all the potential runs
				vector<Combination*> potentialBooks - A list of all the potential books
				vector<Combination*> potentialCombinations - A combination of both vectors listed above. 
/* Algorithm:
			We simply extract potential runs and potential books one at a time and concatenate these vectors into 
			a new vector. 
/* Assistance Received: None

******************************************************************************************************************/
vector<Combination*> HandAnalyzer::extractPotentials(const unsigned table[][Card::M_NUM_RANKS - 2])
{
	vector<Combination*> potentialCombinations;
	vector<Combination*> potentialRuns = extractPotentialRuns(table);
	vector<Combination*> potentialBooks = extractPotentialBooks(table);
	potentialCombinations.insert(potentialCombinations.end(), potentialRuns.begin(), potentialRuns.end());
	potentialCombinations.insert(potentialCombinations.end(), potentialBooks.begin(), potentialBooks.end());
	return potentialCombinations;
}

/*****************************************************************************************************************
/* Function Name: extractRuns
/* Purpose: To obtain runs from a hand of cards
/* Parameters:
			table[][] - The table from which the runs will be extracted.
			wildCardStack - wildcard stack contining the wildcards in hand.
/* Return Value:
			vector<Combination*> runList - returns a vector containing all the possible runs.
/* Local Variables:
			vector<Card> tempVector - Temporary vector to hold cards in potential run.
			Card newCard - A card object that holds the current card.
			Combination* newBook - Hold a run that is created and pushed into the runList.
/* Algorithm:
			We iterate through each suit looking for runs. If there are more than three cards in a sequence in
			a suit, we can call it a run. 
/* Assistance Received: None

******************************************************************************************************************/
// Extracts runs from the table
vector<Combination*> HandAnalyzer::extractRuns(const unsigned table[][Card::M_NUM_RANKS - 2], vector<Card> wildCardStack)
{
	vector<Combination*> runList;
	for (unsigned i = 0; i < Card::M_NUM_SUITS; i++)
	{
		for (unsigned j = 0; j < Card::M_NUM_RANKS - 2; j++)
		{
			unsigned start_index = 0;
			unsigned end_index = 0;

			while (table[i][j] == 0 && j < Card::M_NUM_RANKS - 2)
			{
				start_index++;
				end_index++;
				j++;
			}

			if (m_table[i][j] >= 1 && j < Card::M_NUM_RANKS - 2)
			{
				start_index = j;
				end_index = j;

				while (table[i][j] >= 1 && j < Card::M_NUM_RANKS - 2)
				{
					end_index++;
					j++;
				}

				if (end_index - start_index >= 3)
				{
					vector<Card> tempVector;
					unsigned k = start_index + 3;
					while (k < end_index + 3)
					{
						Card newCard(i + 1, k);
						k++;
						tempVector.push_back(newCard);
					}
					Combination* newRun = new Run(tempVector);
					runList.push_back(newRun);
				}
			}

		}
	}
	return runList;
}


/*****************************************************************************************************************
/* Function Name: extractBooks
/* Purpose: To obtain books from a hand of cards
/* Parameters:
			table[][] - The table from which the books will be extracted.
			wildCardStack - wildcard stack contining the wildcards in hand.
/* Return Value:
				vector<Combination*> bookList - returns a vector containing all the possible books.
/* Local Variables:
				vector<Card> tempVector - Temporary vector to hold cards in potential book.
				Card newCard - A card object that holds the current card. 
				Combination* newBook - Hold a book that is created and pushed into the bookList.
/* Algorithm:
				We iterate through each rank looking for books. If the number of cards in each rank is more than
				three, we can consider it as a book.
/* Assistance Received: None

******************************************************************************************************************/
// Extract books from the table
vector<Combination*> HandAnalyzer::extractBooks(const unsigned table[][Card::M_NUM_RANKS - 2], vector<Card> wildCardStack)
{
	vector<Combination*> bookList;

	// Iterate through each rank
	for (unsigned i = 0; i < Card::M_NUM_RANKS - 2; i++)
	{
		vector<Card> tempVector;
		unsigned counter = 0;

		for (unsigned j = 0; j < Card::M_NUM_SUITS; j++)
		{

			// Continue until a card is found
			while (table[j][i] == 0 && j < Card::M_NUM_SUITS)
			{
				j++;
			}

			// If a card is found, check for a double card.
			// Increase the counter 
			if (table[j][i] >= 1 && j < Card::M_NUM_SUITS)
			{
				Card newCard(j + 1, i + 3);
				if (table[j][i] == 2)
				{
					Card doubleCard(j + 1, i + 3);
					tempVector.push_back(doubleCard);
					counter++;
				}
				tempVector.push_back(newCard);
				counter++;
			}

		}

		// If there are 3 or more cards of the same rank, we push them as a book.
		if (counter >= 3)
		{
			Combination* newBook = new Book(tempVector);
			bookList.push_back(newBook);
		}
	}

	return bookList;
}

/*****************************************************************************************************************
/* Function Name: extractHalfRuns
/* Purpose: To extract half runs (incomplete runs that can be completed with the addition of joker)
/* Parameters:
			table[] - frequency table from which the runs are to be extracted.
			vector<Card> wildCardStack - the stack containing wildcards/jokers.
/* Return Value:
			vector<Combination*> halfRuns - returns a vector of combination containing half runs.
/* Local Variables:
				vector<Combination*> halfRuns - to store the combinations of half runs.
				vector<Card> copyWildCards - a copy of the wildCardStack.
				vector<Card> tempVector - to store temporarily store half runs.
				Card newCard - a card that is created.
				Card tempWildCard - a card to hold a wildcard from the wildcard stack.
				Combination* newRun - a combination object that stores the half runs.
/* Algorithm:
			We iterate through each suit looking for half runs. (cards that are incomplete runs but can be made into runs
			using wildcards).
			As long as we have three or more cards, we can include them as halfRuns.

/* Assistance Received: None

******************************************************************************************************************/
vector<Combination*> HandAnalyzer::extractHalfRuns(const unsigned table[][Card::M_NUM_RANKS - 2], vector<Card> wildCardStack)
{
	vector<Combination*> halfRuns;
	if (!wildCardStack.empty())
	{
		// Iterate through each suit
		for (unsigned i = 0; i < Card::M_NUM_SUITS; i++)
		{
			for (unsigned j = 0; j < Card::M_NUM_RANKS - 2; j++)
			{
				vector<Card> copyWildCards = wildCardStack;
				unsigned counter = 0;
				vector<unsigned> indexHolder;
				vector<Card> tempVector;
				unsigned wildCardCount = copyWildCards.size();

				// Continue until a card is found
				while (table[i][j] == 0 && j < Card::M_NUM_RANKS - 2)
				{
					j++;
				}

				// If a card is found, check if it's adjacent cards are also present
				while (table[i][j] >= 1 && j < Card::M_NUM_RANKS - 2)
				{
					Card newCard(i + 1, j + 3);
					tempVector.push_back(newCard);
					counter++;

					// Handling edge condition
					if (j != Card::M_NUM_RANKS - 3)
					{
						j++;
					}

					// Ignore the 0's and continue until we have wildcards
					while ((table[i][j] == 0 && (!copyWildCards.empty()) && j < Card::M_NUM_RANKS - 2))
					{

						Card tempWildCard = copyWildCards.back();
						wildCardCount--;
						copyWildCards.pop_back();
						tempVector.push_back(tempWildCard);
						counter++;

						// Handling edge cases
						if (j != Card::M_NUM_RANKS - 3)
						{
							j++;
						}
					}


					// If we only have the card in the last rank for a specific suit, 
					// and we have two extra jokers, we need to be able to consider that as 
					// a half run also. So, handle that case.
					while (j == Card::M_NUM_RANKS - 3 && table[i][j] >= 1 && copyWildCards.size() > 0)
					{
						Card tempWildCard = copyWildCards.back();
						wildCardCount--;
						copyWildCards.pop_back();
						tempVector.push_back(tempWildCard);
						counter++;
					}
					if (j == Card::M_NUM_RANKS - 3)
					{
						j++;
					}
				}

				// If there are more than three cards, create a new half run and push it into halfRuns.
				if (counter >= 3)
				{
					Combination* newRun = new HalfRun(tempVector);
					halfRuns.push_back(newRun);
				}

			}
		}
	}

	return halfRuns;

}

/*****************************************************************************************************************
/* Function Name: extractHalfBooks
/* Purpose: To extract half books (incomplete books that can be completed with the addition of joker)
/* Parameters: 
			table[] - frequency table from which the books are to be extracted.
			vector<Card> wildCardStack - the stack containing wildcards/jokers.
/* Return Value:
			vector<Combination*> halfBooks - returns a vector of combination containing half books.
/* Local Variables:
			vector<Combination*> halfBooks - to store the combinations of half books.
			vector<Card> copyWildCard - a copy of the wildCardStack.
			vector<Card> tempVector - to store temporarily store potential half books.
			Card newCard - a card that is created.
			Card doubleCard - a card to store the second card of the same rank and suit.
			Card tempWildCard - a card to hold a wildcard from the wildcard stack. 
			Combination* newBook - a combination object that stores the half book.
/* Algorithm:
			We iterate through each rank looking for half books. (cards that are incomplete books but can be made into books
			using wildcards). 
			As long as we have three or more cards, we can include them as halfRuns.

/* Assistance Received: None

******************************************************************************************************************/
vector<Combination*> HandAnalyzer::extractHalfBooks(const unsigned table[][Card::M_NUM_RANKS - 2], vector<Card> wildCardStack)
{
	vector<Combination*> halfBooks;
	if (!wildCardStack.empty())
	{
		// Iterate through each rank
		for (unsigned i = 0; i < Card::M_NUM_RANKS - 2; i++)
		{
			vector<Card> tempVector;
			vector<Card> copyWildCard = wildCardStack;
			unsigned counter = 0;
			for (unsigned j = 0; j < Card::M_NUM_SUITS; j++)
			{
				// Continue until a card is found
				while (table[j][i] == 0)
				{
					j++;
				}

				// If there is a card, check if there is a double.
				// Continue seraching further in the column
				if (table[j][i] >= 1 && j < Card::M_NUM_SUITS)
				{
					Card newCard(j + 1, i + 3);
					if (table[j][i] == 2)
					{
						Card doubleCard(j + 1, i + 3);
						tempVector.push_back(doubleCard);
						counter++;
					}
					tempVector.push_back(newCard);
					counter++;
				}

			}

			// If there are two or more cards of the same rank,
			// We insert the wildcard and set it as a half book
			if (counter >= 2 && (!copyWildCard.empty()))
			{
				Card tempWildCard = copyWildCard.back();
				copyWildCard.pop_back();
				tempVector.push_back(tempWildCard);
				Combination* newBook = new HalfBook(tempVector);
				halfBooks.push_back(newBook);
			}
		}

	}

	return halfBooks;
}

/*****************************************************************************************************************
/* Function Name: extractPotentialBooks
/* Purpose: To extract potential books (a combination of two cards that could be a book)
/* Parameters: table[] - frequency table from which the books are to be extracted.
/* Return Value:
			vector<Combination*> halfBooks - returns a vector of combination containing potential books.
/* Local Variables:
			vector<Combination*> halfBooks - to store the combinations of potential books.
			vector<Card> tempVector - to store temporarily store potential potential books.
			Card newCard - a card that is created.
			Card doubleCard - a card to store the second card of the same suit and rank.
			Combination* newBook - a combination object that stores the potential books.
/* Algorithm:
			We iterate through each rank looking for potential books (two cards). 
			We just need two cards to make a potential book.
/* Assistance Received: None
******************************************************************************************************************/
vector<Combination*> HandAnalyzer::extractPotentialBooks(const unsigned table[][Card::M_NUM_RANKS - 2])
{
	vector<Combination*> halfBooks;
	
	// Iterate through each rank 
	for (unsigned i = 0; i < Card::M_NUM_RANKS - 2; i++)
	{
		vector<Card> tempVector;
		unsigned counter = 0;
		for (unsigned j = 0; j < Card::M_NUM_SUITS; j++)
		{
			// Continue until we find a card (i.e. value is 1)
			while (table[j][i] == 0)
			{
				j++;
			}
			
			// If the value is greater than 1, see if there is a double card.
			if (table[j][i] >= 1 && j < Card::M_NUM_SUITS)
			{
				Card newCard(j + 1, i + 3);
				if (table[j][i] == 2)
				{
					Card doubleCard(j + 1, i + 3);
					tempVector.push_back(doubleCard);
					counter++;
				}
				tempVector.push_back(newCard);
				counter++;
			}

		}

		// If there two or more cards, create a new potential book. (two should be fine)
		if (counter >= 2)
		{
			Combination* newBook = new HalfBook(tempVector);
			halfBooks.push_back(newBook);
		}
	}

	return halfBooks;
}

/*****************************************************************************************************************
/* Function Name: extractPotentialRuns
/* Purpose: To extract potential runs (a combination of two cards that could be a run)
/* Parameters: table[] - frequency table from which the runs are to be extracted. 
/* Return Value: 
			vector<Combination*> halfRuns - returns a vector of combination containing potential runs. 
/* Local Variables:
			vector<Combination*> halfRuns - to store the combinations of potential runs.
			vector<Card> tempVector - to store temporarily store potential potential runs.
			Card newCard - a card that is created.
			Combination* newRun - a combination object that stores the potential runs.
/* Algorithm:
			We iterate through each suit, trying to find possible runs that have at least two cards in it.
			We consider two cases - 1) two cards in a sequence or 2) two cards with a space in between which be completed
			to make a run.
			
/* Assistance Received: None

******************************************************************************************************************/
vector<Combination*> HandAnalyzer::extractPotentialRuns(const unsigned table[][Card::M_NUM_RANKS - 2])
{
	vector<Combination*> halfRuns;
	
	// We look through the ranks for each suit, trying to find possible runs that have atleast two cards in it. 
	for (unsigned i = 0; i < Card::M_NUM_SUITS; i++)
	{
		for (unsigned j = 0; j < Card::M_NUM_RANKS - 2; j++)
		{
			unsigned counter = 0;
			vector<unsigned> indexHolder;
			vector<Card> tempVector;
			unsigned zeroCounter = 1;

			// If the value in the position is 0, we go to the adjacent position. 
			while (table[i][j] == 0 && j < Card::M_NUM_RANKS - 2)
			{
				j++;
			}

			// If the value is 1 (i.e. we have a card), see if the next value is 0 and the value after that is 1.
			// Only if this is true, we consider it a potential run.
			while (table[i][j] >= 1 && j < Card::M_NUM_RANKS - 2)
			{
				Card newCard(i + 1, j + 3);
				tempVector.push_back(newCard);
				counter++;
				
				// Need to handle the edge case
				if (j != Card::M_NUM_RANKS - 3)
				{
					j++;
				}

				// Check the next index
				if (table[i][j] == 0 && j < Card::M_NUM_RANKS - 2 && zeroCounter != 0)
				{
					counter++;
					zeroCounter--;
					if (j != Card::M_NUM_RANKS - 3)
					{
						j++;
					}
				}

				if (j == Card::M_NUM_RANKS - 3)
				{
					j++;
				}

			}

			// If the number of cards is two or greater, consider that as a potential run. 
			if (counter >= 2)
			{
				Combination* newRun = new HalfRun(tempVector);
				halfRuns.push_back(newRun);
			}

		}
	}

	return halfRuns;

}

/*****************************************************************************************************************
/* Function Name: copyTable
/* Purpose: To make a copy of the table
/* Parameters:
			source[][] - the table which is to be copied. 
			destination[][] - the table that is being copied into. 
/* Return Value: None
/* Local Variables: None
/* Algorithm: Simply iterate through every element in the table and copy it one at a time. 
/* Assistance Received: None

******************************************************************************************************************/
// Copy 2D array
void HandAnalyzer::copyTable(const unsigned source[][Card::M_NUM_RANKS - 2], unsigned destination[][Card::M_NUM_RANKS - 2])
{
	for (unsigned i = 0; i < Card::M_NUM_SUITS; i++)
	{
		for (unsigned j = 0; j < Card::M_NUM_RANKS - 2; j++)
		{
			destination[i][j] = source[i][j];
		}
	}
	return;
}