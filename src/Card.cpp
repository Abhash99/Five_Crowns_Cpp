#include "stdafx.h"
#include "Card.h"


/*****************************************************************************************************************
/* Function Name: isJoker()
/* Purpose: To return if the current card (this object) is a joker or not.
/* Parameters: None
/* Return Value: Boolean values - true if card is joker, false otherwise. 
/* Local Variables: None
/* Algorithm: Check if the card has the joker's suit (6) and return true/false based on the value of the suit. 
/* Assistance Received: None

******************************************************************************************************************/
bool Card::isJoker() const
{
	if (m_suit == 6)
		return true;
	else
		return false;
}

/*****************************************************************************************************************
/* Function Name: Constructor for Card object
/* Purpose: To create a card object from its string representation. This will be used while reading serialized data. 
/* Parameters: 
				a constant string symbol passed by reference, which represents a particular cards. The first character
				in this two character string represents rank while the second character represents suit .
/* Return Value: None
/* Local Variables: None
/* Algorithm: 
				Check the first and second character in the string seperately and determine the rank and suit of the card
				respectively. The first character represents the rank and second represents suit.
				The jokers follow a different representation where the first character represents suit and second character
				is the rank. So, we need to handle jokers seperately from the rest of the card.
			
/* Assistance Received: None

******************************************************************************************************************/
Card::Card(const string &symbol)
{
	// Seond character of the string symbol represents suits
	switch (symbol[1])
	{
	case 'S':
		m_suit = s_spades;
		break;
	case 'H':
		m_suit = s_hearts;
		break;
	case 'C':
		m_suit = s_clubs;
		break;
	case 'D':
		m_suit = s_diamonds;
		break;
	case 'T':
		m_suit = s_tridents;
		break;
	}

	// First character of the string symbol represents rank
	switch (symbol[0])
	{
	case '3':
		m_rank = r_three;
		m_value = m_rank;
		break;
	case '4':
		m_rank = r_four;
		m_value = m_rank;
		break;
	case '5':
		m_rank = r_five;
		m_value = m_rank;
		break;
	case '6':
		m_rank = r_six;
		m_value = m_rank;
		break;
	case '7':
		m_rank = r_seven;
		m_value = m_rank;
		break;
	case '8':
		m_rank = r_eight;
		m_value = m_rank;
		break;
	case '9':
		m_rank = r_nine;
		m_value = m_rank;
		break;
	case 'X':
		m_rank= r_ten;
		m_value = m_rank;
		break;
	case 'J':
		m_rank = r_jack;
		m_value = m_rank;
		break;
	case 'Q':
		m_rank = r_queen;
		m_value = m_rank;
		break;
	case 'K':
		m_rank = r_king;
		m_value = m_rank;
		break;
	}

	// Need to handle Jokers seperately as their symbol appears differently. 
	// Joker is represented as J1, J2, J3
	if (symbol[0] == 'J')
	{
		if (symbol[1] == '1')
		{
			m_suit = s_joker;
			m_rank = 1;
			m_value = 50;
		}

		if (symbol[1] == '2')
		{
			m_suit = s_joker;
			m_rank = 2;
			m_value = 50;
		}

		if (symbol[1] == '3')
		{
			m_suit = s_joker;
			m_rank = 3;
			m_value = 50;
		}
	}

	// Set m_isWildcard as false
	this->m_isWildcard = false;

}

/*****************************************************************************************************************
/* Function Name: toString	
/* Purpose: To obtain the string representation of a card object. 
/* Parameters: None
/* Return Value: Returns the string representation of the card.
/* Local Variables: None
/* Algorithm:
			Checks the suit and rank one at a time and generates the character representation of each of them. 
			Concatenates the two characters to form the complete string representation of the card. 
			Handles jokers differently than the rest of the cards because joker's representation is different; the
			suit comes before the rank. 
/* Assistance Received: None

******************************************************************************************************************/
// Converts the card to string representation
string Card::toString() const
{
	char suit;
	char rank;

	switch (m_suit)
	{
	case s_spades:
		suit = 'S';
		break;
	case s_hearts:
		suit = 'H';
		break;
	case s_clubs:
		suit = 'C';
		break;
	case s_diamonds:
		suit = 'D';
		break;
	case s_tridents:
		suit = 'T';
		break;
	case s_joker:
		suit = 'J';
		break;
	default:
		cerr << "Invalid Suit";
		exit(1);
	}

	// Note: the ranks 1,2 are needed for jokers only.
	switch (m_rank)
	{
	case 1:
		rank = '1';
		break;
	case 2:
		rank = '2';
		break;
	case r_three:
		rank = '3';
		break;
	case r_four:
		rank = '4';
		break;
	case r_five:
		rank = '5';
		break;
	case r_six:
		rank = '6';
		break;
	case r_seven:
		rank = '7';
		break;
	case r_eight:
		rank = '8';
		break;
	case r_nine:
		rank = '9';
		break;
	case r_ten:
		rank = 'X';
		break;
	case r_jack:
		rank = 'J';
		break;
	case r_queen:
		rank = 'Q';
		break;
	case r_king:
		rank = 'K';
		break;
	default:
		cerr << "Invalid Rank";
		exit(1);
	}

	// The string representation for Joker is opposite where suit comes first and rank comes second. Ex. J1, J2, J3
	if (isJoker() == false)
	{
		string symbol(1,rank);
		symbol += suit;
		return symbol;
	}
	else
	{
		string symbol(1, suit);
		symbol += rank;
		return symbol;
	}
}