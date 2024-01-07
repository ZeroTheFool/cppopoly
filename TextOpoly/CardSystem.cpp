#include "CardSystem.h"

void CardSystem::makeChanceDeck()
{
    chanceDeck[0] = std::make_shared<Card>("get 50", "you get 50", 50, 1, functionType::Receive, functionTarget::Bank);
    chanceDeck[1] = std::make_shared<Card>("pay 50", "you pay 50", 50, 1, functionType::Pay, functionTarget::All);
    chanceDeck[2] = std::make_shared<Card>("move to Go", "go to Go", 0, 1, functionType::MoveToIndex, functionTarget::None);
}

void CardSystem::makeCommunityDeck()
{
    communityDeck[0] = std::make_shared<Card>("get 50", "you get 50", 50, 1, functionType::Receive, functionTarget::Bank);
    communityDeck[1] = std::make_shared<Card>("pay 50", "you pay 50", 50, 1, functionType::Pay, functionTarget::Bank);
    communityDeck[2] = std::make_shared<Card>("move to Go", "go to Go", 0, 1, functionType::MoveToIndex, functionTarget::None);
}