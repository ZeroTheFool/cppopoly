#pragma once
/* The purpose of CardSystem is solely to be responsible for the creation of the Community Chest and Chance decks,
* using the GameCard struct, and then return the requested array to the CardSpace class
*/
//#include "Player.h"
//#include "GameAdmin.h"
#include "GameCard.h"
#include "enum.h"

#include <string>
#include <memory>

class Player;
class GameAdmin;

class CardSystem
{
private:
    
    GameCard* chanceDeck[16];
    GameCard* communityDeck[16];
;
public:

    CardSystem() {};
    CardSystem(std::string deckType)
    {
        if (deckType == "chance")
        {
            makeChanceDeck();
        }
        else if (deckType == "community")
        {
            makeCommunityDeck();
        }
    };
    ~CardSystem() {};

    void makeChanceDeck();
    void makeCommunityDeck();

    GameCard* returnChanceCard(int index) {
        return chanceDeck[index];
    };
    GameCard* returnCommunityCard(int index) {
        return communityDeck[index];
    };

};



