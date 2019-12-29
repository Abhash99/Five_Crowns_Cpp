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

class Card {

public:
	// Public Constants
	static const unsigned M_NUM_SUITS = 5;
	static const unsigned M_NUM_RANKS = 13;

	//Public Enumerators
	enum SuitType {
		s_spades = 1,
		s_hearts,
		s_clubs,
		s_diamonds,
		s_tridents,
		s_joker
	};

	enum CardValue {
		r_three = 3,
		r_four,
		r_five,
		r_six,
		r_seven,
		r_eight,
		r_nine,
		r_ten,
		r_jack,
		r_queen,
		r_king
	};

	// Default Constructor
	Card() = default;

	// Constructors
	Card(unsigned suit, unsigned rank)
	{
		this->m_suit = suit;
		this->m_rank = rank;
		this->m_isWildcard = false;

		if (this->m_suit == 6)
		{
			this->m_value = 50;
		}
		else
		{
			this->m_value = rank;

		}
	}

	// Constructor for serialized game data
	Card(const string &symbol);

	// Copy Constructor
	Card(const Card &card)
	{
		this->m_rank = card.getRank();
		this->m_suit = card.getSuit();
		this->m_isWildcard = card.isWildcard();
		this->m_value = card.getValue();
	}

	// == Operator Overloading
	inline bool operator==(const Card &card)
	{
		if ((this->m_rank == card.getRank()) && (this->m_suit == card.getSuit()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// Accessors
	inline unsigned getRank() const
	{
		return this->m_rank;
	}

	inline unsigned getSuit() const
	{
		return this->m_suit;
	}

	inline bool isWildcard() const
	{
		return this->m_isWildcard;
	}

	inline unsigned getValue() const
	{
		return this->m_value;
	}

	// Mutators
	inline void setRank(unsigned rank) 
	{
		m_rank = rank;
	}

	inline void setSuit(unsigned suit)
	{
		m_suit = suit;
	}

	inline void setWildcard(bool isWildcard)
	{
		m_isWildcard = isWildcard;
	}

	inline void setValue(unsigned value)
	{
		m_value = value;
	}

	// Utility Functions
	// Function that determines if the card is a joker or not
	bool isJoker() const;

	// Converts card to string representation
	string toString() const;

private:
	unsigned m_suit;
	unsigned m_rank;
	bool m_isWildcard;
	unsigned m_value;
};