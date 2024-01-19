#include "GameBoard.h"
#include "Property.h"
#include "Railroad.h"
#include "Utility.h"
#include "Tax.h"
#include "Corner.h"
#include "CardSpace.h"

GameBoard::GameBoard()
{
    // name1, name2, purchase, rent 0,1,2,3,4,H, build, build, demo, demo, owner, mort, unmort
    m_newProp = std::make_shared<Property>("Mediterranean", "Avenue", 60, 2, 10, 30, 90, 160, 250, 50, 50, 50, 50, "BANK", 30, 30, Colour::Brown, 3, -1, 1);
    m_boardOrder[1] = m_newProp;

    m_newProp = std::make_shared<Property>("Baltic", "Avenue", 60, 4, 20, 60, 180, 320, 450, 50, 50, 50, 50, "None", 30, 30, Colour::Brown, 1, -1, 3);
    m_boardOrder[3] = m_newProp;


    m_newProp = std::make_shared<Property>("Oriental", "Avenue", 100, 6, 30, 90, 270, 400, 550, 50, 50, 50, 50, "None", 50, 50, Colour::LightBlue, 8, 9, 6);
    m_boardOrder[6] = m_newProp;

    m_newProp = std::make_shared<Property>("Vermont", "Avenue", 100, 6, 30, 90, 270, 400, 550, 50, 50, 50, 50, "None", 50, 50, Colour::LightBlue, 6,9, 8);
    m_boardOrder[8] = m_newProp;

    m_newProp = std::make_shared<Property>("Connecticut", "Avenue", 120, 8, 40, 100, 300, 450, 600, 50, 50, 50, 50, "None", 60, 60, Colour::LightBlue, 6, 8, 9);
    m_boardOrder[9] = m_newProp;


    m_newProp = std::make_shared<Property>("St Charles", "Place", 140, 10, 50, 150, 450, 625, 750, 100, 100, 100, 100, "None", 70, 70, Colour::Magenta, 13, 14, 11);
    m_boardOrder[11] = m_newProp;

    m_newProp = std::make_shared<Property>("States", "Avenue", 140, 10, 50, 150, 450, 625, 750, 100, 100, 100, 100, "None", 70, 70, Colour::Magenta, 11, 14, 13);
    m_boardOrder[13] = m_newProp;

    m_newProp = std::make_shared<Property>("Virginia", "Avenue", 160, 12, 60, 180, 500, 700, 900, 100, 100, 100, 100, "None", 80, 80, Colour::Magenta, 11, 13, 14);
    m_boardOrder[14] = m_newProp;


    m_newProp = std::make_shared<Property>("St James", "Place", 180, 14, 70, 200, 550, 750, 95, 100, 100, 100, 100, "None", 90, 90, Colour::Orange, 18, 19, 16);
    m_boardOrder[16] = m_newProp;

    m_newProp = std::make_shared<Property>("Tennessee", "Avenue", 180, 14, 70, 200, 550, 750, 95, 100, 100, 100, 100, "None", 90, 90, Colour::Orange, 16, 19, 18);
    m_boardOrder[18] = m_newProp;

    m_newProp = std::make_shared<Property>("New York", "Avenue", 200, 16, 80, 220, 600, 800, 1000, 100, 100, 100, 100, "None", 100, 100, Colour::Orange, 16, 18, 19);
    m_boardOrder[19] = m_newProp;


    m_newProp = std::make_shared<Property>("Kentucky", "Avenue", 220, 18, 90, 250, 700, 875, 1050, 150, 150, 150, 150, "None", 110, 110, Colour::Red, 23, 24, 21);
    m_boardOrder[21] = m_newProp;

    m_newProp = std::make_shared<Property>("Indiana", "Avenue", 220, 18, 90, 250, 700, 875, 1050, 150, 150, 150, 150, "None", 110, 110, Colour::Red, 21, 24, 23);
    m_boardOrder[23] = m_newProp;

    m_newProp = std::make_shared<Property>("Illinois", "Avenue", 240, 20, 100, 300, 750, 925, 1100, 150, 150, 150, 150, "None", 120, 120, Colour::Red, 21, 23, 24);
    m_boardOrder[24] = m_newProp;


    m_newProp = std::make_shared<Property>("Atlantic", "Avenue", 260, 24, 110, 330, 800, 975, 1150, 150, 150, 150, 150, "None", 130, 130, Colour::Yellow, 27, 29, 26);
    m_boardOrder[26] = m_newProp;

    m_newProp = std::make_shared<Property>("Ventor", "Avenue", 260, 24, 110, 330, 800, 975, 1150, 150, 150, 150, 150, "None", 130, 130, Colour::Yellow, 26, 29, 27);
    m_boardOrder[27] = m_newProp;

    m_newProp = std::make_shared<Property>("Marvin", "Gardens", 280, 24, 120, 360, 850, 1025, 1200, 150, 150, 150, 150, "None", 140, 140, Colour::Yellow, 26, 27, 29);
    m_boardOrder[29] = m_newProp;


    m_newProp = std::make_shared<Property>("Pacific", "Avenue", 300, 26, 130, 390, 900, 1100, 1275, 200, 200, 200, 200, "None", 150, 150, Colour::Green, 32, 34, 31);
    m_boardOrder[31] = m_newProp;

    m_newProp = std::make_shared<Property>("North Carolina", "Avenue", 300, 26, 130, 390, 900, 1100, 1275, 200, 200, 200, 200, "None", 150, 150, Colour::Green, 31, 34, 32);
    m_boardOrder[32] = m_newProp;

    m_newProp = std::make_shared<Property>("Pennsylvania", "Avenue", 320, 28, 150, 390, 900, 1200, 1400, 200, 200, 200, 200, "None", 160, 160, Colour::Green, 31, 32, 34);
    m_boardOrder[34] = m_newProp;


    m_newProp = std::make_shared<Property>("Park", "Place", 350, 35, 175, 500, 1100, 1300, 1500, 200, 200, 200, 200, "None", 175, 175, Colour::Blue, 39, -1, 37);
    m_boardOrder[37] = m_newProp;

    m_newProp = std::make_shared<Property>("Boardwalk", "", 400, 50, 200, 600, 1400, 1700, 2000, 200, 200, 200, 200, "None", 200, 200, Colour::Blue, 37, -1, 39);
    m_boardOrder[39] = m_newProp;


    // name1, name2, purchase, owner, mort, unmort
    m_newRail = std::make_shared<Railroad>("Reading", "Railroad", 200, "None", 100, 100, 5);
    m_boardOrder[5] = m_newRail;

    m_newRail = std::make_shared<Railroad>("Pennsylvania", "Railroad", 200, "None", 100, 100, 15);
    m_boardOrder[15] = m_newRail;

    m_newRail = std::make_shared<Railroad>("B & O", "Railroad", 200, "None", 100, 100, 25);
    m_boardOrder[25] = m_newRail;

    m_newRail = std::make_shared<Railroad>("Short", "Line", 200, "None", 100, 100, 35);
    m_boardOrder[35] = m_newRail;


    // name1, name2, purchase, owner, mort, unmort
    m_newUtility = std::make_shared<Utility>("Water", "Works", 150, "None", 75, 75, 12);
    m_boardOrder[12] = m_newUtility;

    m_newUtility = std::make_shared<Utility>("Electric", "Company", 150, "None", 75, 75, 28);
    m_boardOrder[28] = m_newUtility;


    // corners can't be bought, they will never go into a player's property list, so they don't need a board location
    // it will be enough to have them in boardOrder correctly
    m_newCorner = std::make_shared<Corner>("GO", "", false, "Collect $200 here", 0); // L0
    m_boardOrder[0] = m_newCorner;

    m_newCorner = std::make_shared<Corner>("Just", "Visiting", false, "Visiting Jail", 10); // L10
    m_boardOrder[10] = m_newCorner;

    m_newCorner = std::make_shared<Corner>("Free", "Parking", false, "No effect", 20); // L20
    m_boardOrder[20] = m_newCorner;

    m_newCorner = std::make_shared<Corner>("Go To", "Jail", true, "Go to Jail", 30); // L30
    m_boardOrder[30] = m_newCorner;

    m_newCorner = std::make_shared<Corner>("In", "Jail", false, "Currently In Jail", 40); // L40
    m_boardOrder[40] = m_newCorner;


    // Community Chest and Chance spaces have no individual uniqueness
    // put a pointer to a single instance in three places
    // these spaces don't need a board location, board location is only for ownable spaces
    // as long as they exist in m_boardOrder then it's fine
    // EDIT: until they get deleted, because one the first instance is deleted, the other two cause errors
    m_newSpace = std::make_shared<CardSpace>("Community", "Chest", "chance"); //TESTING: third argument should be community DON'T CHANGE
    m_boardOrder[2] = m_newSpace;
    m_boardOrder[17] = m_newSpace;
    m_boardOrder[33] = m_newSpace;

    m_newSpace = std::make_shared<CardSpace>("Chance", "", "chance");
    m_boardOrder[7] = m_newSpace;
    m_boardOrder[22] = m_newSpace;
    m_boardOrder[36] = m_newSpace;


    m_newTax = std::make_shared<Tax>("Income", "Tax", 200, 0);
    m_boardOrder[4] = m_newTax;

    m_newTax = std::make_shared<Tax>("Luxury", "Tax", 100, 0);
    m_boardOrder[38] = m_newTax;
}

std::shared_ptr<BoardSpace> GameBoard::getCurrentSpace(int index)
{
    return m_boardOrder[index];
}

//void GameBoard::deletePointers()
//{
//    for (int i = 0; i < c_boardSize; i++)
//    {
//        if (i != 17 && i != 33 && i != 22 && i != 36)
//        {
//            // skips duplicate chance/chest indexes
//            // because i "saved resources" by using the same pointer for all three instances of each
//            // after the first one is deleted, the other two can't even be checked to see if they exist
//            delete m_boardOrder[i]; // this has a read access violation at i = 17
//        }
//    }
//}

