#pragma once
#include "Player.h"
#include "GameAdmin.h"
#include "GameCard.h"
#include "enum.h"

#include <string>
#include <memory>

class Player;

class CardSystem
{
private:
    
    std::shared_ptr<GameCard> chanceDeck[16];
    std::shared_ptr<GameCard> communityDeck[16];
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
    std::shared_ptr<GameCard> returnChanceCard(int index) {
        return chanceDeck[index];
    };
    std::shared_ptr<GameCard> returnCommunityCard(int index) {
        return communityDeck[index];
    };

};



