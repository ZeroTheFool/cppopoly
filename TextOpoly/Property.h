#pragma once
#include "BoardSpace.h"
#include "enum.h"
//#include "Player.h"

#include <string>
class Player;


class Property : 
    public BoardSpace
{
private:

    Colour m_setColour = Colour::None;
    std::string m_colourDisplay = "";

    int m_houses = 0;
    bool m_hotel = false;

    int m_rentFee[6] = { 25, 50, 75, 100, 125, 175 }; // {0 = with no houses, 1/2/3/4 = with # houses, 5 = with hotel}

    int m_houseBuildPrice = 0;
    int m_houseDemolishValue = 0;

    int m_hotelBuildPrice = 0;
    int m_hotelDemolishValue = 0;

    int m_setNeighbours[2] = { -1, -1 }; // this will contain the other two (or one) property in the same colour set
    // the purpose of this will be to allow the player class to check ownership of a set and/or tell all members they are complete
    // dark blue and brown will only have one index that isn't -1
    bool m_fullSet = false;
    int m_setSize = 3;

public:
    Property() {};
    Property(std::string inStreetName, std::string inStreetType, int inPurchasePrice, int fee0, int fee1, int fee2, int fee3, int fee4, int fee5, int inHouseBuild, int inHotelBuild, int inHouseDemo, int inHotelDemo, std::string inOwner, int inMortgage, int inUnmortgage, Colour inColour, int neighbourA, int neighbourB, int inBoardLocation = 0)
        : BoardSpace (inStreetName, inStreetType, inPurchasePrice, inOwner, inMortgage, inUnmortgage, inBoardLocation)
    {        
        m_rentFee[0] = fee0;
        m_rentFee[1] = fee1;
        m_rentFee[2] = fee2;
        m_rentFee[3] = fee3;
        m_rentFee[4] = fee4;
        m_rentFee[5] = fee5;
        m_houseBuildPrice = inHouseBuild;
        m_hotelBuildPrice = inHotelBuild;
        m_houseDemolishValue = inHouseDemo;
        m_hotelDemolishValue = inHotelDemo;

        m_setColour = inColour;
        colourAsString(m_setColour);
        m_setNeighbours[0] = neighbourA;
        m_setNeighbours[1] = neighbourB;
        if (m_setNeighbours[1] = -1)
        {
            m_setSize = 2;
        }
    };
    
    int checkBuidCost(); // to display make sure the player can afford to build, then build()  
    bool build();   // returns true if something was built, the player will be charged the amount returned from CheckBuildCost()
                    // return false if nothing can be built, and no money will be spent
    int demolish(); // returns the value of the demolished structure, no need to check the value elsewhere
                    // demolish cost will be shown in DisplayDetails() for UI purposes

    void displayDetails();
    void viewMenu(int houses, int hotels);

    int getRentFee();

    int houseCount();

    Colour getSetColour();
    void colourAsString(Colour newColour);
    int getSetSize();
    void updateSetStatus(bool isComplete);
    int getNeighbourByIndex(int index);

    bool canBeMortgaged();
    bool canBuildHere();
};

