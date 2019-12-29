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
#include "Combination.h"
#include "Card.h"
#include "Hand.h"
#include "Book.h"
#include "Run.h"
#include "HalfBook.h"
#include "HalfRun.h"

class HandAnalyzer
{
public:

	// Constructor and Destructor
	HandAnalyzer();
	~HandAnalyzer(){}

	// Accessors

	inline vector<Combination*> getCombination() const
	{
		return this->m_bestCombination;
	}

	inline vector<Card> getRemainingCards() const
	{
		return this->m_remainingCards;
	}

	inline vector<Card> getWildCardStack() const
	{
		return this->m_wildCardStack;
	}

	// Mutators
	inline void setCombination(vector<Combination*> cardObjects)
	{
		this->m_bestCombination = cardObjects;
	}

	inline void setRemainingCards(vector<Card> remainingCards)
	{
		this->m_remainingCards = remainingCards;
	}

	inline void setWildCardStack(vector<Card> wildCardStack)
	{
		this->m_wildCardStack = wildCardStack;
	}


	// Utility Functions
	void loadHandsToTable(Hand playerHand);
	void loadWildCards(Hand playerHand);
	void analyzeHand(Hand playerHand);


	vector<Card> getRemainingVector(Hand playerHand, vector<Combination*> bestCombination);
	vector<Combination*> getBestCombination(Hand playerHand, vector<vector<Combination*>> combinationCollection);
	unsigned getRemainingCardsNum(Hand playerHand, vector<Combination*> newCombination);

	void getAllCombinations(Hand playerHand, vector<Combination*> allCombinations, const unsigned table[][Card::M_NUM_RANKS - 2], vector<Card> &wildCardStack, vector<Combination*> &helper, vector<vector<Combination*>> &combinationCollection);
	void eraseFromTable(Combination* combination, unsigned table[][Card::M_NUM_RANKS - 2]);

	vector<Combination*> considerPotentials(Hand remainingCards);

	vector<Combination*> extractCardObjects(const unsigned table[][Card::M_NUM_RANKS - 2], vector<Card> wildCardStack);
	vector<Combination*> extractPotentials(const unsigned table[][Card::M_NUM_RANKS - 2]);

	vector<Combination*> extractRuns(const unsigned table[][Card::M_NUM_RANKS - 2], vector<Card> wildCardStack);
	vector<Combination*> extractBooks(const unsigned table[][Card::M_NUM_RANKS - 2], vector<Card> wildCardStack);
	vector<Combination*> extractHalfRuns(const unsigned table[][Card::M_NUM_RANKS - 2], vector<Card> wildCardStack);
	vector<Combination*> extractHalfBooks(const unsigned table[][Card::M_NUM_RANKS - 2], vector<Card> wildCardStack);
	vector<Combination*> extractPotentialBooks(const unsigned table[][Card::M_NUM_RANKS - 2]);
	vector<Combination*> extractPotentialRuns(const unsigned table[][Card::M_NUM_RANKS - 2]);

	void copyTable(const unsigned source[][Card::M_NUM_RANKS - 2], unsigned destination[][Card::M_NUM_RANKS - 2]);

private:
	unsigned m_table[Card::M_NUM_SUITS][Card::M_NUM_RANKS - 2] = { 0 };
	vector<Combination*> m_bestCombination;
	vector<Card> m_remainingCards;
	vector<Card> m_wildCardStack;
};

