#pragma once
//#include "Player.h"
#include <memory>
#include <vector>

class Player;

struct tradeOffer
{
    int tradeMoney = 0;
    int tradeJailCards = 0;
    std::vector<std::shared_ptr<BoardSpace>> tradeProperties;
    std::shared_ptr<Player> trader;
};