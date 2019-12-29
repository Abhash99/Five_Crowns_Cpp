#include "stdafx.h"
#include "Human.h"

/*****************************************************************************************************************
/* Function Name: printMenu
/* Purpose: To print the menu before each turn
/* Parameters: None
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			Print the menu before human's turn
/* Assistance Received: None

******************************************************************************************************************/
void Human::printMenu()
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
unsigned Human::getMenuInput()
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
/* Purpose: To initiate the human player's turn
/* Parameters:
			vector<Card> drawPile and vector<Card> discardPile - both passed by reference.
			This is because we need to update drawPile and discardPile when the player picks and
			drops cards.
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			Display player's hand. Enables player to pick a card either from draw or discard pile, drop a card from
			hand and go out when possible. Help can also be provided by the computer in each stage of the game. (i.e.
			pick, drop, arrange card and go out)
/* Assistance Received: None
******************************************************************************************************************/
void Human::play(vector<Card> &drawPile, vector<Card> &discardPile)
{
	cout << endl;
	cout << getName() << ":" << endl;
	m_hand.displayHand();
	pickCard(drawPile, discardPile);
	m_hand.displayHand();
	dropCard(discardPile);
	cout << endl;

	// Ask if player needs help arranging cards or wants to go out or continue play
	unsigned choice;
	do
	{
		cout << "What would you like to do: " << endl;
		cout << "================================" << endl;
		cout << "\t 1. Continue Play" << endl;
		cout << "\t 2. Go out" << endl;
		cout << "\t 3. Ask help to arrange cards" << endl;
	
		cout << "Enter your choice: ";
		cin >> choice;

		if (choice == 3)
		{
			helpArrange();
		}

	} while (!(choice >= 1 && choice <= 2));

	if (choice == 1)
	{
		cout << "Continuing Play..." << endl;
		return;
	}
	else
	{
		goOut();
	}

	return;
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
void Human::askForCall(char &choice)
{
	char input;
	do
	{
		cout << this->getName() << ", Please call heads(H) or Tails(T): ";
		cin >> input;
		input = tolower(input);
	} while (!(input == 'h' || input == 't'));

	choice = input;
	return;
}

/*****************************************************************************************************************
/* Function Name: help pick
/* Purpose: Provide recommendations to the human player to pick a card using AI strategy (HandAnaluyzer)
/* Parameters:
			vector<Card> discardPile - Need to look at the card on top of the discard pile in order to give help.
/* Return Value: None	
/* Local Variables:
				Hand playerHand - copy of human's hand
				temp - temporary copy of human's hand to analyze it seond time
				HandAnalyzer analyzer1 - Used to analyze players hand and get the best run/books and remaining cards
				HandAnalyzer analyzer2 - Used to analyzer player hand after adding card from discard pile
/* Algorithm:
				We use handAnalyzer object to determine our current number of remaining cards. We then add the card from
				discard pile and analyze our hand again. If the new number of remaining cards is equal or less than the previous
				remaining cards, recommend to pick from discard pile. Else, recommend to pick from draw pile.
/* Assistance Received: None

******************************************************************************************************************/
void Human::helpPick(vector<Card> discardPile)
{
	Hand playerHand = getHand();
	Hand temp = playerHand;

	// We need new analyzer for every check that we do
	// New analyzer
	HandAnalyzer analyzer1;
	analyzer1.analyzeHand(playerHand);
	unsigned currentRemaining = analyzer1.getRemainingCards().size();

	Card topDiscard = discardPile.back();
	temp.addCardToHand(topDiscard);

	//New analyzer to check if discard pile will help
	HandAnalyzer analyzer2;
	analyzer2.analyzeHand(temp);
	unsigned newRemaining = analyzer2.getRemainingCards().size();

	// If adding the top card from draw pile results in less remaining cards, choose discard else choose draw.
	// Note: <= because when we pick a card from discard, our hand has one extra card. So, even if we get the
	// same number of remaining cards, we are in a better position. 
	if (newRemaining <= currentRemaining)
	{
		cout << "I recommend you to pick from discard because you use the top card to make a run/book." << endl;
	}
	else
	{
		cout << "I recommend you to pick from draw pile because the top card in discard pile is not useful." << endl;
	}

	return;
}

/*****************************************************************************************************************
/* Function Name: helpDrop
/* Purpose: To help the human to drop a card using AI strategy
/* Parameters: None
/* Return Value: Nones
/* Local Variables:
					Card toDrop - the variable that holds the recommended card to drop.
					Hand playerhand - copy of player's hand
					HandAnalyzer analyzer - Used to analyzer player's hand for runs/book  and remaining cards.
					vector<Card> remainingCardStack - vector to hold the remaining cards after setting aside runs/books
					Hand remaining - A new hand to store the remaining cards as a hand and analyze them.
					HandAnalyzer potentialAnalyzer - Another HandAnalyzer object to analyze the hand of remaining cards to identify
					potential runs/books.
					vector<Combination*> potentials - vector of Combination pointers that stores all the potential runs/books.
/* Algorithm:
			Analyze player's hand - get remaining cards after extracting runs/books.
			Then, we create a hand of remaining cards and further analyze them to identify potential books/runs. We
			don't want the player to discard useful cards.
			We remove the cards that are can form potential books/runs from the hand.
			We find the greatest card among the remaining of the cards and recommend to discard the greatest card.
/* Assistance Received: None

******************************************************************************************************************/
void Human::helpDrop()
{
	Card toDrop;
	Hand playerHand = getHand();
	// New Analyzer to check which card to drop
	HandAnalyzer analyzer;
	analyzer.analyzeHand(playerHand);
	vector<Card> remainingCardStack = analyzer.getRemainingCards();

	// We need to consider potential books/runs that could be in remaining cards
	// so we create a temporary hand object to extract those potential cards
	Hand remaining;
	if (remainingCardStack.size() > 0)
	{
		remaining.setHand(remainingCardStack);
		// New analyzer to check the potential runs and books in the remaining cards
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
				toDrop = Computer::findGreatestCard(remaining.getHand());
			}
			// If remaining is empty, we will have to abandon the potential run/book and drop a card
			else
			{
				toDrop = Computer::findGreatestCard(remainingCardStack);
			}

		}
		else
		{
			toDrop = Computer::findGreatestCard(remainingCardStack);
		}
	}
	else
	{
		cout << "All cards are in combinations. You could drop any card. " << endl;
		return;
	}

	cout << "I recommend you to drop " << toDrop.toString() << " because it is the greatest unused card in your hand. " << endl;

	return;
}

/*****************************************************************************************************************
/* Function Name: helpArrange
/* Purpose: To help the human player to arrange their cards in runs/books using AI strategy
/* Parameters: None
/* Return Value: None
/* Local Variables:
				Hand playerHand - copy of human's hand
				HandAnalyzer analyzer - Analyzer object to analyze current hand and get best combinations.
				vector<Combination*> bestCombination - holds the best combination for player's hand.
/* Algorithm:
			We use the HandAnalyzer object to first find the best combinations in player's hand and we print the best
			combinations to the screen. 
/* Assistance Received: None

******************************************************************************************************************/
void Human::helpArrange()
{
	Hand playerHand = getHand();
	HandAnalyzer analyzer;
	analyzer.analyzeHand(playerHand);
	vector<Combination*> bestCombinations = analyzer.getCombination();

	if (!bestCombinations.empty())
	{
		for (auto comb : bestCombinations)
		{
			cout << "I recommend you to ";
			comb->printPile();
			cout << endl;
		}
	}
	else
	{
		cout << "You don't have any runs and books at this stage." << endl;
	}

	return;
}



/*****************************************************************************************************************
/* Function Name: pickCard
/* Purpose: To allow human to pick from either draw or discard pile
/* Parameters:
			vector<Card> drawPile - passed by reference because we need to update it after picking.
			vector<Card> discardPile - also passed by reference because we need to update it after picking. 
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			Display user's option. Prompt the user for input. Get the input from the user and based on the input, use the
			pickFromDraw / pickFromDiscard functions to pick a card. 
/* Assistance Received: None

******************************************************************************************************************/
void Human::pickCard(vector<Card> &drawPile, vector<Card> &discardPile)
{
	unsigned choice;
	// Print options to pick. Ask if player wants to pick from draw pile, discard pile or wants to ask computer for help. 
	// Continue only if choice is 1 or 2, which means player wants to pick either from drop or discard. 
	do
	{
		cout << "Would you like to pick a card from draw pile or discard pile?" << endl;
		cout << "\t 1. Draw Pile" << endl;
		cout << "\t 2. Discard Pile" << endl;
		cout << "\t 3. Ask for help" << endl;
		cin >> choice;

		if (choice == 3)
		{
			cout << "Asking Computer for Help: " << endl;
			cout << "=====================================" << endl;
			helpPick(discardPile);
			cout << endl;
		}

	} while (!(choice >= 1 && choice <= 2));

	if (choice == 1)
	{
		this->pickFromDraw(drawPile);
	}
	else
	{
		this->pickFromDiscard(discardPile);
	}

	return;
}

/*****************************************************************************************************************
/* Function Name: dropCard
/* Purpose: To allow human player to drop a card
/* Parameters: vector<Card> discardPile - passed by reference because we will need to drop the card to discard pile.
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			We simply display human player's options. Prompt them for input and based on their input, drop a card to
			discard pile. Help can also be provided to drop a card. 
/* Assistance Received: None

******************************************************************************************************************/
void Human::dropCard(vector<Card> &discardPile)
{
	unsigned choice;

	// Print options (whether to discard or ask for help)
	// Obtain user input and only continue when input == 1, which means player wants to discard a card.
	do
	{
		
		cout << "What would you like to do: " << endl;
		cout << "===========================" << endl;
		cout << "1. Discard Card" << endl;
		cout << "2. Ask for Help" << endl;

		
		cout << "Enter your choice: ";
		cin >> choice;

		if (choice == 2)
		{
			cout << "Asking Computer for Help: " << endl;
			cout << "=====================================" << endl;
			helpDrop();
			cout << endl;
		}
	} while (choice != 1);


	// Ask for the card that the player wants to discard. Input should be the string symbol of the card as represented in hand.
	// Please note this input has very limited validation. It has to be uppercase letters and numbers combination. If the card is not present,
	// the program crashes.
	string cardString;
	Card toBeRemoved;

	cout << "Please enter the card you would like to discard: ";
	cin >> cardString;
	cout << endl;

	// Remove the card from player's hand. 
	toBeRemoved = this->m_hand.removeCard(cardString);
	cout << "Human removed " << cardString << " from hand." << endl;
	discardPile.push_back(toBeRemoved);
}


/*****************************************************************************************************************
/* Function Name: pickFromDraw
/* Purpose: Add a card from drawPile to hand
/* Parameters:
			vector<Card> drawPile - passed by reference to get the card and remove it from top of the drawPile
/* Return Value: None
/* Local Variables: Card topCard - to store the top card in the drawPile and add it later to hand
/* Algorithm:
			We store the top card in the drawPile in a temporary variable. Remove the card from the drawPile and
			add the card to human's hand.
/* Assistance Received: None

******************************************************************************************************************/
void Human::pickFromDraw(vector<Card> &drawPile)
{
	Card topCard = drawPile.back();
	drawPile.pop_back();
	addCard(topCard);
}

/*****************************************************************************************************************
/* Function Name: pickFromDiscard
/* Purpose: Add a card from discardPile to hand
/* Parameters:
			vector<Card> discardPile - passed by reference to get the card and remove it from top of the discardPile
/* Return Value: None
/* Local Variables: 
			Card topCard - to store the top card in the discardPile and add it later to hand.
/* Algorithm:
			We store the top card in the discardPile in a temporary variable. Remove the card from the discardPile and
			add the card to human's hand.
/* Assistance Received: None
******************************************************************************************************************/
void Human::pickFromDiscard(vector<Card> &discardPile)
{
	Card topCard = discardPile.back();
	discardPile.pop_back();
	addCard(topCard);
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
void Human::lastTurn(vector<Card> &drawPile, vector<Card> &discardPile)
{
	cout << endl;
	cout << getName() << ":" << endl;
	m_hand.displayHand();
	pickCard(drawPile, discardPile);
	m_hand.displayHand();
	dropCard(discardPile);
	cout << endl;
	m_hand.displayHand();
	evaluateHand();
}
