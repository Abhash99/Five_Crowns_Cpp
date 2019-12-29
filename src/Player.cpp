
#include "stdafx.h"
#include "Player.h"

/*****************************************************************************************************************
/* Function Name: Player
/* Purpose: Constructor for player class
/* Parameters:
			string name - name of the player
			unsigned playerNum - player number
/* Return Value: None
/* Local Variables: None
/* Algorithm: Simply initialize all the member variables.
/* Assistance Received: None

******************************************************************************************************************/
Player::Player(string name, unsigned playerNum)
{ 
	this->m_name = name;
	this->m_playerNum = playerNum;
	this->m_score = 0;
	this->m_goOutFlag = false;

}

/*****************************************************************************************************************
/* Function Name: play	
/* Purpose: To initiate playing turn of each player
/* Parameters: vector<Card> drawPile and discardPile - passed by reference to be updated by the player's turn
/* Return Value: None
/* Local Variables: None
/* Algorithm: It is a virtual function in the base class. 
/* Assistance Received: None

******************************************************************************************************************/
void Player:: play(vector<Card> &drawPile, vector<Card> &discardPile)
{
}	

/*****************************************************************************************************************
/* Function Name: addCard
/* Purpose: To add card to player's hands
/* Parameters: Card c
/* Return Value: None
/* Local Variables: None
/* Algorithm:
			Simply call the addCardToHand function of Hand Class.
/* Assistance Received: None

******************************************************************************************************************/
void Player::addCard(Card c)
{
	this->m_hand.addCardToHand(c);
}

/*****************************************************************************************************************
/* Function Name: clearHand
/* Purpose: To clear player's hand
/* Parameters: None
/* Return Value: None
/* Local Variables: None
/* Algorithm: Simply calls the clear hand function of Hand class.
/* Assistance Received: None

******************************************************************************************************************/
void Player::clearHand()
{
	this->m_hand.clearHand();
}


/*****************************************************************************************************************
/* Function Name: goOut
/* Purpose: To make the player go out
/* Parameters: None
/* Return Value: None
/* Local Variables:
				HandAnalyzer analyzer - Used to analyze player's hand to determine if the player can go out.
				vector<Card> remiainingCards - holds the remaining cards in player's hand after setting aside
				runs/books.
/* Algorithm:
			We analyze the player's hand and obtain the remaining cards that are in player's hand. If there are no 
			remaining cards, we can declare that player went out. Then set the goOutFlag and evalute player's hand.
			If the remaining cards are all jokers, we can go out, otherwise we cannot go out if we have any other
			remaining cards.

/* Assistance Received: None

******************************************************************************************************************/
void Player::goOut()
{
	// Verify if the player can go out
	HandAnalyzer analyzer;
	analyzer.analyzeHand(getHand());
	vector<Card> remainingCards = analyzer.getRemainingCards();
	unsigned remCards = remainingCards.size();
	if (remCards == 0)
	{
		cout << getName() << " went out. " << endl;
		setGoOutFlag(true);
		evaluateHand();
		return;
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
				cout << "You cannot go out at this point. Your cards cannot be arranged into books/runs." << endl;
				return;
			}
		}
		cout << getName() << " went out. " << endl;
		cout << "================================================" << endl;
		setGoOutFlag(true);
		evaluateHand();
		return;
	}

}

/*****************************************************************************************************************
/* Function Name: evaluateHand
/* Purpose: To evaluate the runs/books and remaining cards at the end of the round when a player went out.
/* Parameters: None
/* Return Value: None
/* Local Variables:
				Hand Analyzer analyzer - To analyze player's hand for runs and books.
				vector<Combination*> bestCombination - Best possible combination of runs and books in player's hand. 
				vector<Card> remainingCard - Remaining cards in player's hand after setting runs/books aside. 
				vector
/* Algorithm:
			We analyze player's hand using a HandAnalyzer object and obtain the bestCombination and remaining cards. 
			Print all the runs/books the player has. If there are any potential combinations (i.e. books/runs of two cards)
			We add their rank value to the total score. In the remaining cards, we add the rank value of all the cards to the 
			player's score. If the card is a joker, it has 50 point, if is it a wildcard, it gets 20 points.
			At the end, print the player's total score for the round.
/* Assistance Received: None

******************************************************************************************************************/
void Player::evaluateHand()
{
	unsigned scoreCounter = 0;
	HandAnalyzer analyzer;
	analyzer.analyzeHand(getHand());
	vector<Combination*> bestCombination = analyzer.getCombination();
	vector<Card> remainingCard = analyzer.getRemainingCards();

	// Print all the combinations that the player has
	if (!bestCombination.empty())
	{
		
		for (auto comb : bestCombination)
		{
			cout << getName() << " can ";
			comb->printPile();
			cout << endl;
		}

		cout << endl;
	}
	else
	{
		cout << getName() << " doesn't have any combinations in hand." << endl;
	}

	// Need to consider potential runs/books (which are of size 2)
	for (auto comb : bestCombination)
	{
		if (comb->getPile().size() == 2)
		{
			for (auto card : comb->getPile())
			{
				card.setValue(card.getRank());
				scoreCounter = scoreCounter + card.getValue();
			}
		}
	}

	if (remainingCard.size() > 0)
	{
		cout << getName() << "'s remaining cards are: ";
		for (auto card : remainingCard)
		{
			if (card.isJoker())
			{
				card.setValue(50);
			}
			else if (card.isWildcard())
			{
				card.setValue(20);
			}
			else
			{
				card.setValue(card.getRank());
			}
			scoreCounter = scoreCounter + card.getValue();
			cout << card.toString() << " ";
		}
		cout << endl;
	}
	else
	{
		cout << endl;
		cout << getName() <<" doesn't have any remaining cards." << endl;
		cout << endl;
	}

	cout << getName() << "'s score for this round is: " << scoreCounter << "." << endl;
	setScore(getScore() + scoreCounter);
}