# Five_Crowns_Cpp

## Five Crowns
Five Crowns is a card game played by 2 players.

## The Objective
The objective of this game is to score the fewest points after all the rounds.


## The Players
Two players play this game - one player will be the human user of your program, and the other player will be your program/computer.

## The Setup
Two 58-card decks are used. Each deck contains:
- five suites: spades, clubs, diamonds, hearts and tridents.
- eleven cards in each suit: 3 through 10, Jack, Queen and King. Note no Ace, 1 or 2 cards.
- three jokers.

The values of the cards are as follows:
- Cards 3-10 are worth their face value.
- Jack is worth 11 points, Queen is 12 and King is 13.
- Joker is worth 50 points.
- Each round has a wild card. The wild card of a round is the card with the face value equal to the number of cards dealt in the round, e.g., in a round where players are dealt 7 cards each, 7 is the wild card. The wild card is worth 20 points.

## A Round
A round proceeds as follows:
- The two decks are shuffled together.
- Each player is dealt n cards. On the first round, n = 3. On the next round, n = 4, and so on. On the last round, n = 13.
- For each round, the wild card is the card with the value of n.
- The remaining cards are placed in draw pile face down. The top card in the draw pile is removed from the draw pile and placed face up in discard pile.
- First player:
- On the first round, a coin is tossed to determine who plays first. Human player is asked to call the toss.
- On subsequent rounds, the player to go out first on the previous round plays first.
The two players take alternate turns thereafter till the round ends.
- The round ends when one player goes out and the other player plays a turn to earn points.

## A Turn
During her/his turn, a player plays as follows:
- Draws the top card from the draw pile or the top card from the discard pile.
- Tries to assemble the cards in hand as runs and books:
- A run is a sequence of three or more cards of the same suit, e.g., 5, 6 and 7 of diamonds.
- A book is three or more cards of the same value, e.g., 9 of clubs, clubs and tridents.
Each card can be part of only one book or run.
Jokers and wild cards make it easier to assemble runs and books:

- A joker can stand in for any card in a run or book, e.g., a run can be 5 of diamonds, a joker standing for 6 of diamonds and 7 of diamonds.
- A wild card can stand in for any card in a run or book, e.g., on the first round when the wild card is 3, a book can be 9 of clubs, 3 of any suit and 9 of tridents.
The player can have any number of wild cards and jokers in a run or book, and they can be anywhere in a run. A Joker or wild card can be moved from one book/run to another between turns, e.g., a Joker used as part of a 3-4-5 run on turn 1 can be re-purposed to be part of a 7-7-7 book on the next turn if the player picks up a 3, 4 or 5 card during turn 1.
- Discard one card on to the discard pile.
- If all the remaining cards in the player's hand can be arranged in runs and/or books, the player will go out by displaying the runs and books.
- If the other player has just gone out, the player will display all the runs and books in her/his hand. The sum of the values of the remaining cards in the player's hand will be the points awarded to the player.

## A Game
A game consists of 11 rounds:
- In the first round, each player is dealt 3 cards and the wild card is 3.
- In each subsequent round, the number of cards dealt to the players and therefore, the wild card increases by 1.
- In the last round, the cards dealt to each player is 13, and the wild card is King (13).

## Score
A player's score is the sum of the points earned by the player on all 11 rounds. Note that a player who goes out first in a round earns 0 points. The winner of the game is the player with the lowest score.

