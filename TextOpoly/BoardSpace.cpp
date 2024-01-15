#include "BoardSpace.h"
#include <iostream>

void BoardSpace::createDisplayName()
{
    // add top border to display name
    m_displayName = "x--------------------x\n";

    m_displayName += "|" + formatDisplayNameRow(m_streetName, 20) + "|\n";;
    m_displayName += "|" + formatDisplayNameRow("", 20) + "|\n";;
    m_displayName += "|" + formatDisplayNameRow(m_streetType, 20) + "|\n";;

    // add bottom border to display name
    m_displayName += "x--------------------x\n";
}

std::string BoardSpace::formatDisplayNameRow(std::string text, int width)
{
    std::string preSpace = "";
    std::string postSpace = "";
    size_t totalSpaces = 0;
    std::string m_newRow = "";

    totalSpaces = width - text.length(); // the longest name is North Carolina, plus one space on either side

    // the name needs spaces added between the border and text
    for (int i = 0; i < (totalSpaces / 2); i++) // divide the number of needed spaces between the area pre and post the name
    {
        preSpace += " ";
        postSpace += " ";
    }

    if (totalSpaces % 2 == 1) // number of spaces was not even
    {
        postSpace += " "; // add an extra space after the name
    }

    // add needed spaces and name to display
    m_newRow += preSpace + text + postSpace;
    
    return m_newRow;
}

bool BoardSpace::operator==(const BoardSpace& space)
{
    return (m_streetName == space.m_streetName);
}

int BoardSpace::getBoardLocation()
{
    return m_boardLocation;
}

std::string BoardSpace::getFirstName()
{
    // unless you can compare objects, i need this to compare two properties to remove from vectors
    return m_streetName;
}

void BoardSpace::printName()
{
    std::cout << m_displayName << std::endl;
}

void BoardSpace::setOwner(std::string player)
{
    m_currentOwner = player;
}

std::string BoardSpace::getOwner()
{
    return m_currentOwner;
}

int BoardSpace::getMortgageAmount()
{
    return m_mortgageAmount; ;
}

int BoardSpace::getUnMortgageAmount()
{
    return m_unMortgageAmount;
}

bool BoardSpace::switchMortgageState()
{
    m_isMortgaged = !m_isMortgaged;

    return m_isMortgaged;
}

bool BoardSpace::checkMortgaged()
{
    return m_isMortgaged;
}

int BoardSpace::getPurchasePrice()
{
    return m_purchasePrice;
}

bool BoardSpace::isTradeListed()
{
    return m_tradeListed;
}

void BoardSpace::updateTradeList()
{
    m_tradeListed = !m_tradeListed;
}
