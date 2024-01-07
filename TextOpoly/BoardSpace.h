#pragma once
//#include "Player.h"
#include <string>

enum class Colour
{
    None,
    Brown,
    LightBlue,
    Magenta,
    Orange,
    Red,
    Yellow,
    Green,
    Blue
};

class BoardSpace
{
protected:
    std::string m_streetName = "";
    std::string m_streetType = "";
    std::string m_displayName = "";
    void createDisplayName();
    // x----------------x
    // x North Carolina x
    // x     Avenue     x
    // x----------------x

    int m_purchasePrice = 0;
    
    std::string m_currentOwner = "None";
    //Player* m_currentOwner = nullptr;

    int m_mortgageAmount = 0;
    int m_unMortgageAmount = 0;
    bool m_isMortgaged = false;

    int m_boardLocation = 0;

    bool m_tradeListed = false;

public:

    BoardSpace() {};
    BoardSpace(std::string inStreetName, std::string inStreetType, int inPurchasePrice, std::string inCurrentOwner, int inMortgageAmt, int inUnmortgageAmt, int inBoardLocation = 0)
    {
        m_streetName = inStreetName;
        m_streetType = inStreetType;

        m_mortgageAmount = inMortgageAmt;
        m_unMortgageAmount = inUnmortgageAmt;

        m_currentOwner = inCurrentOwner;

        m_purchasePrice = inPurchasePrice;

        m_boardLocation = inBoardLocation;

        createDisplayName();
    };
    virtual ~BoardSpace() {};

    std::string formatDisplayNameRow(std::string text, int width);

    bool operator==(const BoardSpace& space);

    int getBoardLocation();

    std::string getFirstName();
    void printName();

    void setOwner(std::string player);
    std::string getOwner();

    virtual int checkBuidCost() { return 0; }; // to display make sure the player can afford to build, then build()  
    virtual bool build() { return false; };   // returns true if something was built, the player will be charged the amount returned from CheckBuildCost()
    // return false if nothing can be built, and no money will be spent
    virtual int demolish() { return 0; }; // returns the value of the demolished structure, no need to check the value elsewhere
    // demolish cost will be shown in DisplayDetails() for UI purposes

    int getMortgageAmount();
    int getUnMortgageAmount();
    virtual bool switchMortgageState(); // true becomes false, false becomes true
    bool checkMortgaged();

    virtual void displayDetails() {};

    virtual int getRentFee(int rollSum) { return 0; }; // for Utility
    virtual int getRentFee() { return 0; }; // for non-Utility

    virtual int getPurchasePrice();

    // returns 0 if no options are printed
    // returns 1 if ony mortgage is possible
    // returns 3 if build/demolish also available (houseCount will enable player class to error check options)
    virtual void viewMenu(int houses, int hotels) { };
    virtual int viewMenu() { return 0; };

    virtual void setRails(int newVal) {};
    virtual void setUtility(int newVal) {};

    virtual bool checkIsSendJail() { return false; };

    virtual Colour getSetColour() { return Colour::None; };
    void colourAsString(Colour newColour) { };
    
    virtual int houseCount() { return 0; };

    virtual bool canBeMortgaged() { return false; };
    virtual bool canBuildHere() { return false; };

    virtual int getTaxAmount() { return 0; };
    bool isTradeListed();
    void updateTradeList();
};


