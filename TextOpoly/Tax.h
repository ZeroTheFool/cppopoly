#pragma once
#include "BoardSpace.h"
class Tax :
    public BoardSpace
{
private:
    int m_taxAmount = 0;

public:
    Tax() {};
    Tax(std::string inStreetName, std::string inStreetType, int inTaxAmount, int inBoardLocation = 0)
        : BoardSpace(inStreetName, inStreetType, 0, "BANK", 0, 0, inBoardLocation)
    {
        m_taxAmount = inTaxAmount;
    };

    void displayDetails();

    int getRentFee();
};

