#pragma once
#include "BoardSpace.h"
#include "Property.h"
#include "Railroad.h"
#include "Utility.h"
#include "Tax.h"
#include "Corner.h"
#include "CardSpace.h"
//#include "CardSystem.h"

#include <vector>
#include <string>
#include <iostream>
#include <memory>

class Property;

//class CardSystem;

class GameBoard
{
private:
    static const int c_boardSize = 41;
    std::shared_ptr<BoardSpace> m_boardOrder[c_boardSize];

    std::shared_ptr <BoardSpace> m_newSpace;
    std::shared_ptr <Property> m_newProp;
    std::shared_ptr <Railroad> m_newRail;
    std::shared_ptr <Utility> m_newUtility;
    std::shared_ptr <Tax> m_newTax;
    std::shared_ptr <Corner> m_newCorner;

    //CardSystem cSystem;

public:
    GameBoard();
    ~GameBoard() {};
    std::shared_ptr<BoardSpace> getCurrentSpace(int index);

    //void deletePointers();
};

