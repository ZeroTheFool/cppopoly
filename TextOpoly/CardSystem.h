#pragma once
#include "Player.h"
#include "GameAdmin.h"
#include "enum.h"

#include <string>
#include <memory>

class Player;

class Card
{
public:
    std::string m_name;
    std::string m_description;

    int m_functionValue = 0; // index for moving, amount for paying
    int m_payMultiplier = 1; // normally 1, but may be 2 or 10
    functionTarget m_target = functionTarget::Bank;
    functionType m_type = functionType::Pay;

    Card() {};
    Card(std::string aName, std::string aDescription, int value, int multiplier, functionType aType, functionTarget aTarget)
    {
        m_name = aName;
        m_description = aDescription;

        m_functionValue = value;
        m_payMultiplier = multiplier;
        m_type = aType;
        m_target = aTarget;
    };
    ~Card() {};
};

class CardSystem
{
private:
    
    std::shared_ptr<Card> chanceDeck[16];
    std::shared_ptr<Card> communityDeck[16];
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
    std::shared_ptr<Card> returnChanceCard(int index) {
        return chanceDeck[index];
    };
    std::shared_ptr<Card> returnCommunityCard(int index) {
        return communityDeck[index];
    };

};



