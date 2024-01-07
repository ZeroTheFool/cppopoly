#pragma once
#include "BoardSpace.h"
class Railroad :
    public BoardSpace
{
private:
    static const int c_rentVals = 5;
    int m_rails = 1;
    int m_rentFee[c_rentVals] = { 0, 25, 50, 100, 200 }; // {1/2/3/4 owned by same player}

public:
    Railroad() {};
    Railroad(std::string inStreetName, std::string inStreetType, int inPurchasePrice, std::string inOwner, int inMortgage, int inUnmortgage, int inBoardLocation = 0)
        : BoardSpace(inStreetName, inStreetType, inPurchasePrice, inOwner, inMortgage, inUnmortgage, inBoardLocation)
    {
    };

    void displayDetails();
    int viewMenu();

    int getRentFee();

    void setRails(int newVal);
};

