#pragma once
#include "enum.h"

#include <string>

struct GameCard
{
    std::string m_name;
    std::string m_description;

    int m_functionValue = 0; // index for moving, amount for paying
    int m_payMultiplier = 1; // normally 1, but may be 2 or 10
    functionTarget m_target = functionTarget::Bank;
    functionType m_type = functionType::Pay;

    GameCard() {};
    GameCard(std::string name, std::string description, int value, int multiplier, functionTarget target, functionType type) {};
};

