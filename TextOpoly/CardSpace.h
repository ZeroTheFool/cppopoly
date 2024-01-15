#pragma once
#include "BoardSpace.h"
#include "CardSystem.h"
#include "GameCard.h"
#include "Player.h"

#include <memory>
#include <array>

class CardSpace :
    public BoardSpace
{
private:
    std::array<GameCard*,16> deck;
    CardSystem* cSystem = nullptr;
    int deckPosition = 0;

public:
    CardSpace() {};
    CardSpace(std::string inStreetName, std::string inStreetType, std::string spaceType)
        : BoardSpace(inStreetName, inStreetType, 0, "Bank", 0, 0, 0)
    {
        // when a chance or community chest gets created, need to create the associated deck
        if (spaceType == "chance")
        {
            cSystem = new CardSystem("chance");
            for (int i = 0; i < 16; i++)
            {
                deck = cSystem->returnChanceCard();
            }
        }
        else if (spaceType == "community")
        {
            cSystem = new CardSystem("community");
            for (int i = 0; i < 3; i++)
            {
                deck[i] = cSystem->returnCommunityCard(i);
            }
        }
        shuffleDeck();
    };
    ~CardSpace() {};

    void shuffleDeck();
    GameCard* drawNextCard();
};


