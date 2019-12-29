#include "stdafx.h"
#include "Computer.h"



/*****************************************************************************************************************
/* Function Name: printMenu
/* Purpose: To print the menu before each turn
/* Parameters: None
/* Return Value: None
/* Local Variables: None
/* Algorithm: 
			Print the menu before computer's turn
/* Assistance Received: None

******************************************************************************************************************/
void Computer::printMenu()
{
	cout << "Options: " << endl;
	cout << "\t 1. Save the game" << endl;
	cout << "\t 2. Make a move" << endl;
	cout << "\t 3. Quit the game" << endl;
}

/*****************************************************************************************************************
/* Function Name: getMenuInput	
/* Purpose: To get the human input for the menu above
/* Parameters: None
/* Return Value: Unsigned integer that represents the user's choice
/* Local Variables: None
/* Algorithm:
			Prompt the user for input. Only accept input between 1 and 3
/* Assistance Received: None

******************************************************************************************************************/
unsigned Computer::getMenuInput()
{
	unsigned option;
	do
	{
		cout << "Please enter your option: ";
		cin >> option;
	} while (!(option >= 1 && option <= 3));

	// Quit game if option is 3. 
	if (option == 3)
	{
		cout << "Exiting Program! Byee!!" << endl;
		exit(0);
	}
	return option;
}

/*****************************************************************************************************************
/* Function Name: play
/* Purpose: To initiate the computer player's turn
/* Parameters: 
			vector<Card> drawPile and vector<Card> discardPile - both passed by reference.
			This is because we need to update drawPile and discardPile when the player picks and 
			drops cards.
/* Return Value: None
/* Local Variables: None	
/* Algorithm:
			Display player's hand. Computer player uses AI strategy to pick the card, drop the card and go out if 
			it can. 
/* Assistance Received: None

******************************************************************************************************************/
void Computer::play(vector<Card> &drawPile, vector<Card> &discardPile)
{
	// Print player's name and hand
	cout << endl;
	cout << getName() << ":" << endl;
	m_hand.displayHand();
	cout << endl;

	pickCard(drawPile, discardPile);

	m_hand.displayHand();
	
	dropCard(discardPile);

	if (canGoOut() == true)
	{
		goOut();
	}
	cout << endl;
}

/*****************************************************************************************************************
/* Function Name: askForCall
/* Purpose: To ask for player's input for coin toss
/* Parameters:
			A character choice that is passed by reference
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			This is a virtual function in the base class and doesn't have any functionality for the computer's
			class. 
/* Assistance Received: None

******************************************************************************************************************/
void Computer::askForCall(char &choice)
{
	return;
}

/*****************************************************************************************************************
/* Function Name: pickCard
/* Purpose: To pick a card from either draw or discard pile using AI strategy
/* Parameters: vector<Card> drawPile and discardPile - Both are passed by reference in order to update those piles
				after picking a card.
/* Return Value: None
/* Local Variables:
				Hand temp - temporary copy of the player's hand that will be analyzed
				HandAnalyzer analyzer - HandAnalyzer object to analyze player's hand
				Card topDiscard - that gets the copy of the top of discardPile
				HandAnalyzer analyzer2 - HandAnalyzer object to analyze hand after adding card from discard pile. 
/* Algorithm:
				Analyze the cards in hand using the handAnalyzer and obtain the number of remaining cards.
				Add the card in discard pile to hand, analyze the hand again and get new number of remaining
				cards.
				If the new remaining cards is less than or equals the first remaining card, pick the card from the
				discard pile. 
				Else pick the card from draw pile.
				
/* Assistance Received: None

******************************************************************************************************************/
void Computer::pickCard(vector<Card> &drawPile, vector<Card> &discardPile)
{
	// Analyze the current hand and get current remaining. 
	Hand temp = getHand();
	HandAnalyzer analyzer;
	analyzer.analyzeHand(temp);
	unsigned currentRemaining = analyzer.getRemainingCards().size();

	// Add card from the top of discard pile, analyze hand and get the new remaining cards
	Card topDiscard = discardPile.back();
	temp.addCardToHand(topDiscard);
	HandAnalyzer analyzer2;
	analyzer2.analyzeHand(temp);
	unsigned newRemaining = analyzer2.getRemainingCards().size();
	

	// If there are less remaining cards after adding the top of discard pile, pick from discard pile.
	// Else pick from draw pile
	if (newRemaining <= currentRemaining)
	{
		pickFromDiscard(discardPile);
		cout << "Computer picked from discard pile because it can make a run/book. ";
		cout << endl;
	}
	else
	{
		pickFromDraw(drawPile);
		cout << "Computer picked from draw pile because top card in discard pile isn't helpful." << endl;
	}
	return;
}

/*****************************************************************************************************************
/* Function Name: dropCard	
/* Purpose: To drop a card from the computer's hand
/* Parameters: 
				vector<Card> discardPile - passed by reference to update the discard pile when the player drops a 
				card.
/* Return Value: None
/* Local Variables:
				Card toBeRemoved - A card object that holds the card returned by cardToDrop function.
/* Algorithm:
				First, obtain the card that needs to be obtained from cardToDrop function.
				Then, obtain the string representation of the card
				Remove the card from the hand. Print the rationale and adds the card to discard pile.
/* Assistance Received:

******************************************************************************************************************/
void Computer::dropCard(vector<Card> &discardPile)
{
	Card toBeRemoved = cardToDrop();
	string cardString = toBeRemoved.toString();
	toBeRemoved = this->m_hand.removeCard(cardString);
	cout << "Computer removed " << cardString << " from hand because it was the greatest card in hand." << endl;
	discardPile.push_back(toBeRemoved);
}

/*****************************************************************************************************************
/* Function Name: cardToDrop
/* Purpose: To return the optimal card that can be dropped from the hand
/* Parameters: None
/* Return Value: 
			Card toDrop - One of the card in the hand that is unused and can be dropped.
/* Local Variables:
			Hand temp - A copy of the player's hand
			HandAnalyzer analyzer - Handanalyzer object to analyze the hand.
			vector<Card> remainingCardStack - stores the copy of the possible remaining cards after analyzing the
			hand. 
			Hand remaining - A new hand to store the remaining cards as a hand and analyze them.
			HandAnalyzer potentialAnalyzer - Another HandAnalyzer object to analyze the hand of remaining cards to identify
			potential runs/books.
			vector<Combination*> potentials - vector of Combination pointers that stores all the potential runs/books.
/* Algorithm:
			Analyze player's hand - get remaining cards after extracting runs/books.
			Then, we create a hand of remaining cards and further analyze them to identify potential books/runs. We
			don't want the computer to discard useful cards.
			We remove the cards that are can form potential books/runs from the hand.
			We find the greatest card among the remaining of the cards and discard the greatest card. 
/* Assistance Received: None

******************************************************************************************************************/
Card Computer::cardToDrop()
{
	Card toDrop;
	Hand temp = getHand();
	HandAnalyzer analyzer;
	analyzer.analyzeHand(temp);
	vector<Card> remainingCardStack = analyzer.getRemainingCards();

	// We need to consider potential books/runs that could be in remaining cards
	// so we create a temporary hand object to extract those potential cards
	Hand remaining;
	if (remainingCardStack.size() > 0)
	{
		remaining.setHand(remainingCardStack);
		HandAnalyzer potentialAnalyzer;
		vector<Combination*> potentials = potentialAnalyzer.considerPotentials(remaining);
		if (potentials.size() > 0)
		{
			for (unsigned i = 0; i < potentials.size(); i++)
			{
				for (auto card : potentials[i]->getPile())
				{
					remaining.removeCard(card.toString());
				}
			}

			// Remaining now has all the cards except for the potential runs or books
			// If remaining has card in it, find the greatest card and return it
			if (remaining.getHand().size() > 0)
			{
				toDrop = findGreatestCard(remaining.getHand());
			}
			// If remaining is empty, we will have to abandon the potential run/book and drop a card
			else
			{
				toDrop = findGreatestCard(remainingCardStack);
			}

		}
		else
		{
			toDrop = findGreatestCard(remainingCardStack);
		}
	}
	else
	{
		cerr << "Some error occured. No cards to remove. " << endl;
		exit(1);
	}

	return toDrop;
}

/*****************************************************************************************************************
/* Function Name: pickfromDraw
/* Purpose: To add top card from drawPile to hand
/* Parameters: vector<Card> drawPile - passed by reference in order to update the drawPile after picking from it. 
/* Return Value: None
/* Local Variables: 
				Card topCard - stores the top card in the drawPile.
/* Algorithm:
			We obtain the top card from the draw pile and store it in the variable.
			We remove the card from drawPile and add the card into player's hand.
/* Assistance Received: None

******************************************************************************************************************/
void Computer::pickFromDraw(vector<Card> &drawPile)
{
	Card topCard = drawPile.back();
	drawPile.pop_back();
	addCard(topCard);
	return;
}

/*****************************************************************************************************************
/* Function Name: pickFromDiscard
/* Purpose: To pick the card from discard pile and add it into player's hand
/* Parameters: vector<Card> discardPile - passed by reference to update the pile after picking from it. 
/* Return Value: None	
/* Local Variables: Card topCard - stores the top card of the discardPile.
/* Algorithm:
			Obtain the top card of the discardPile.
			Remove the card from the discard pile and add the card into player's hand.
/* Assistance Received: None

******************************************************************************************************************/
void Computer::pickFromDiscard(vector<Card> &discardPile)
{
	Card topCard = discardPile.back();
	discardPile.pop_back();
	addCard(topCard);
}

/*****************************************************************************************************************
/* Function Name: canGoOut
/* Purpose: To determine if a player can go out with current hand
/* Parameters: None
/* Return Value: Boolean - true if player can go out, false if player can't go out.
/* Local Variables:
				HandAnalyzer analyzer - to analyze player's hand.
				vector<Card> remainingCards - to store remaining cards after extracting combination.
/* Algorithm:
				Analyze player's hand using handanalyzer and get the remaining cards after extracting all combinations.
				If the size of remainingCard vector is 0, this means all cards can be arranged in runs/books.
				So, the player can go out and then return true.
				Else, if remainingCard vector has cards that are not jokers, then return false. 

/* Assistance Received: None

******************************************************************************************************************/
bool Computer::canGoOut()
{
	HandAnalyzer analyzer;
	analyzer.analyzeHand(getHand());
	vector<Card> remainingCards = analyzer.getRemainingCards();
	unsigned remCards = remainingCards.size();
	if (remCards == 0)
	{
		return true;
	}
	else
	{
		for (unsigned i = 0; i < remCards; i++)
		{
			while (remainingCards[i].isJoker() || remainingCards[i].isWildcard())
			{
				i++;
			}
			
			if (!(remainingCards[i].isJoker() || remainingCards[i].isWildcard()))
			{
				return false;
			}
		}
		return true;
	}

}

/*****************************************************************************************************************
/* Function Name: lastTurn
/* Purpose: To play the last turn after one of the player goes out. 
/* Parameters:
				vector<Card> drawPile and discardPile - passed by reference in order to update the piles after the player
				pick or drops card.
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			Display Player's Hand, Pick a card (either from draw or discard), drop a card from hand and finally,
			evaluate hand (because it is the last round).
/* Assistance Received: None

******************************************************************************************************************/
void Computer::lastTurn(vector<Card> &drawPile, vector<Card> &discardPile)
{
	cout << endl;
	cout << getName() << ":" << endl;
	m_hand.displayHand();
	cout << endl;
	pickCard(drawPile, discardPile);
	m_hand.displayHand();
	dropCard(discardPile);
	m_hand.displayHand();
	evaluateHand();
}

/*****************************************************************************************************************
/* Function Name: findGreatestCards
/* Purpose: To return the greatest card in a vector of cards	
/* Parameters: vector<Card> cardStack - the vector that contains the cards
/* Return Value: Card greatest - which is the card with the highest rank in the vector
/* Local Variables:
/* Algorithm:
			Set max = 0.
			Iteratively, go through all the cards in the vector, comparing its rank to the max value.
			If card's rank is greater than max, set max to the card's rank, and set that card as the greatest.
			At the end of the iteration, we should have the greatest card. 
/* Assistance Received: None

******************************************************************************************************************/
Card Computer::findGreatestCard(vector<Card> cardStack)
{
	Card greatest;
	unsigned max = 0;
	for (auto card : cardStack)
	{
		if (card.getRank() > max)
		{
			max = card.getRank();
			greatest = card;
		}
	}

	return greatest;
}