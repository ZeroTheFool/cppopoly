#include "Property.h"
#include <iostream>
#include <Windows.h>

using namespace std;

//void Property::createDisplayName()
//{
//    // add top border to display name
//    displayName = "x----------------x";
//
//    std::string preSpace = "";
//    std::string postSpace = "";
//    size_t totalSpaces = 0;
//    
//    totalSpaces = 16 - streetName.length(); // the longest name is North Carolina, plus one space on either side
//
//    // the name needs spaces added between the border and text
//    for (int i = 0; i < (totalSpaces / 2); i++) // divide the number of needed spaces between the area pre and post the name
//    {
//        preSpace += " ";
//        postSpace += " ";
//    }
//
//    if (totalSpaces % 2 == 1) // number of spaces was not even
//    {
//        postSpace += " "; // add an extra space after the name
//    }
//    
//    // add needed spaces and name to display
//    displayName += "\nx" + preSpace + streetName + postSpace + "x\nx";
//    
//    preSpace = "";
//    postSpace = "";
//    totalSpaces = 16 - streetType.length(); // the longest name is North Carolina, plus one space on either side
//
//    // the type needs spaces added between the border and text
//    for (int i = 0; i < (totalSpaces / 2); i++) // divide the number of needed spaces between the area pre and post the name
//    {
//        preSpace += " ";
//        postSpace += " ";
//    }
//
//    if (totalSpaces % 2 == 1) // number of spaces was not even
//    {
//        postSpace += " "; // add an extra space after the name
//    }
//    
//    
//    displayName += preSpace + streetType + postSpace;
//
//    // add bottom border to display name
//    displayName += "x\nx----------------x";
//}

int Property::checkBuidCost()
{
    if (m_houses < 4)
    {
        return m_houseBuildPrice; // to build with 0-3 houses, you would pay the house price
    }
    else
    {
        return m_hotelBuildPrice; // to build with 4 houses, you would pay the hotel price
    }
}

void Property::displayDetails()
{
    if (m_isMortgaged)
        cout << formatDisplayNameRow("--MORTGAGED--", 18) << endl;

    if (m_tradeListed)
        cout << formatDisplayNameRow("--ON TRADE--", 18) << endl;

    cout << formatDisplayNameRow("SET: " + m_colourDisplay + "\n", 18) << endl;
    // could there be an indicator here that player owns full set?

    if (m_currentOwner == "None")
    {
        cout << formatDisplayNameRow("Price:" + to_string(m_purchasePrice), 18) << endl;
        cout << formatDisplayNameRow("Rent:" + to_string(m_rentFee[m_houses]), 18) << endl;
    }
    else
    {
        cout << formatDisplayNameRow("Owner:" + BoardSpace::m_currentOwner, 18) << endl;
        if (!m_isMortgaged)
            cout << formatDisplayNameRow("Rent:" + to_string(m_rentFee[m_houses]), 18) << endl;
    }

    if (m_hotel)
    {
        cout << formatDisplayNameRow("Hotel", 18) << endl;
    }
    else
    {
        cout << formatDisplayNameRow("Houses:" + to_string(m_hotel ? 0 : m_houses), 18) << endl; // if a hotel is present, have to say 0 houses
    }


    cout << "\nx----------------x\n" << endl;
}

void Property::viewMenu(int houses, int hotels)
{
    if (!m_isMortgaged) // not mortgaged, can build
    {
        if (m_houses < 4 && houses > 0) // player has room to build houses, and houses are available
        {
            cout << "b - build (-$" << m_houseBuildPrice << ")" << endl;
        }
        else if (m_houses == 4 && hotels > 0) // player has room to build hotel, hotels are available
        {
            cout << "b - build (-$" << m_hotelBuildPrice << ")" << endl;
        }
        else
        {
            cout << "Can't build here" << endl;
        }

        if (m_houses < 5 && m_houses > 0)
        {
            cout << "d - demolish (+$" << m_houseDemolishValue << ")" << endl;
        }
        else if (m_houses == 5)
        {
            cout << "d - demolish(+$" << m_hotelDemolishValue << ")" << endl;
        }
        else
        {
            cout << "Can't demolish here" << endl;
        }
    }

    if (!m_isMortgaged && m_houses == 0)
    {
        cout << "m - mortgage" << "(+$" << m_mortgageAmount << ")" << endl;
    }
    else if (!m_isMortgaged && m_houses > 0)
    {
        cout << "Can't mortgage with buildings" << endl;
    }
    else if (m_isMortgaged) // if mortgaged, can unmortgage
    {
        cout << "m - unmortgage" << "(-$" << m_unMortgageAmount << ")" << endl;
    }

    cout << "======================" << endl;
}

int Property::getRentFee()
{
    // return fee element using number of houses as index
    return m_rentFee[m_houses];
}

int Property::houseCount()
{
    return m_houses;
}

Colour Property::getSetColour()
{
    return m_setColour;
}

void Property::colourAsString(Colour newColour)
{
    switch (newColour)
    {
    case Colour::Brown:
        m_colourDisplay = "BR";
        break;
    case Colour::Blue:
        m_colourDisplay = "BL";
        break;
    case Colour::LightBlue:
        m_colourDisplay = "LB";
        break;
    case Colour::Magenta:
        m_colourDisplay = "MG";
        break;
    case Colour::Orange:
        m_colourDisplay = "OR";
        break;
    case Colour::Red:
        m_colourDisplay = "RD";
        break;
    case Colour::Yellow:
        m_colourDisplay = "YL";
        break;
    case Colour::Green:
        m_colourDisplay = "GR";
        break;
    default:
        m_colourDisplay = "XX";
        break;
    }
}

bool Property::canBeMortgaged()
{
    if (m_isMortgaged) // if it is mortgaged now, it can be unmortgaged
    {
        return true;
    }
    if (!m_isMortgaged && m_houses == 0) // if it is unmortgaged and there are no houses, it can be mortgaged
    {
        return true;
    }
    return false; // probably only possibility here is unmortgaged with 1+ houses
}

bool Property::canBuildHere()
{
    return (m_houses < 5 && !m_isMortgaged);
}

bool Property::build() // the player has already confirmed the price, and can afford it
{
    if (!m_hotel)
    {
        if (m_houses < 4) // don't have 4 houses yet, can buy one
        {
            m_houses++;
            return true;
        }
        else // have 4 houses, buy hotel
        {
            m_hotel = true;
            m_houses++; // the number of hotels is used for the rentFee array
            return true;
        }
    }
    // if hotel already owned, can't buy
    return false;
}

int Property::demolish()
{
    if (!m_hotel) // don't have a hotel
    {
        if (m_houses > 0) // do have houses to be demolished
        {
            m_houses--;
            return m_houseDemolishValue;
        }
    }
    else // do have a hotel
    {
        m_hotel = false;
        m_houses--;
        return m_hotelDemolishValue;
    }
    return 0;
}
