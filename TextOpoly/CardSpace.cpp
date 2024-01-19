#include "CardSpace.h"
#include "Player.h"

void CardSpace::shuffleDeck()
{
    GameCard* temp;
    //for (int i = 0; i < 16; i++)
    for (int i = 0; i < 3; i++)
    {
        // for each card, swap positions with a random other card
        temp = deck[i];
        int swap = rand() % 16;
        deck[i] = deck[swap];
        deck[swap] = temp;
    }
}

GameCard* CardSpace::drawNextCard()
{
    // this function just returns the next card
    // and increments the draw index
    // admin handles the rest
    GameCard* nextCard = deck[deckPosition];
    
    if (deckPosition < 15)
    {
        deckPosition++;
    }
    else
    {
        deckPosition = 0;
        shuffleDeck();
    }

    return nextCard;
}
