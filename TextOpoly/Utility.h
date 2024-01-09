#pragma once
#include "BoardSpace.h"

class Utility :
    public BoardSpace
{
private:

    int utilitiesOwned = 1;
    int rentMultipier[3] = { 0, 4, 10 }; // the [0] will never actually be used, but this is eaiser than two variables

public:
    Utility() {};
    Utility(std::string inStreetName, std::string inStreetType, int inPurchasePrice, std::string inOwner, int inMortgage, int inUnmortgage, int inBoardLocation = 0)
        : BoardSpace(inStreetName, inStreetType, inPurchasePrice, inOwner, inMortgage, inUnmortgage, inBoardLocation)
    {
        
    };

    void displayDetails();
    int viewMenu();

    void setUtility(int newVal);

    int getRentFee(int rollSum);
};

