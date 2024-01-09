#pragma once
#include "BoardSpace.h"
//#include "CardSystem.h"
#include "Player.h"

#include <memory>

class CardSystem;
class Card;


class CardSpace :
    public BoardSpace
{
private:
    std::shared_ptr<Card> deck[16] = { 0 };
    //CardSystem* cSystem = nullptr;
    int deckPosition = 0;

public:
    CardSpace() {};
    CardSpace(std::string inStreetName, std::string inStreetType, std::string spaceType)
        : BoardSpace(inStreetName, inStreetType, 0, "Bank", 0, 0, 0)
    {
        // when a chance or community chest gets created, need to create the associated deck
        /*if (spaceType == "chance")
        {
            cSystem = new CardSystem("chance");
            for (int i = 0; i < 3; i++)
            {
                deck[i] = cSystem->returnChanceCard(i);
            }
        }
        else if (spaceType == "community")
        {
            cSystem = new CardSystem("community");
            for (int i = 0; i < 3; i++)
            {
                deck[i] = cSystem->returnCommunityCard(i);
            }
        }*/
    };
    ~CardSpace() {};

    void shuffleDeck();
    std::shared_ptr<Card> drawNextCard();
};


