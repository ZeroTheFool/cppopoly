#pragma once
//#include "BoardSpace.h"
//#include "GameAdmin.h"
#include "enum.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

class BoardSpace;
class GameAdmin;
struct tradeOffer;

class Player
{
private:
    std::shared_ptr<GameAdmin> m_admin;
    static const int c_boardSize = 40;
    int m_totalMoney = 0;
    int m_jailCards = 0; // 0, 1, 2 "get out of jail free" cards, subtract 1 to escape
    std::shared_ptr<Player> m_inDebtToPlayer;
    
    bool m_isBankrupt = false;
    bool m_isInJail = false;
    int m_turnsInJail = 0;
    bool m_hasBonusRoll = false;
    
    int m_dieRoll1 = 0;
    int m_dieRoll2 = 0;

    int m_propertyCounter = 0;
    int houseCounter = 0;
    int hotelCounter = 0;

    std::map<Colour, int> m_setCollection = {
        {Colour::Brown,0}, 
        {Colour::LightBlue,0},
        {Colour::Magenta,0},
        {Colour::Orange,0},
        {Colour::Red,0},
        {Colour::Yellow,0},
        {Colour::Green,0},
        {Colour::Blue,0}
    };


    // this holds 39 spaces, of which the player can only actually own 28
    // the array starts full of nullptr, and acquired properties will be added in the index matching their board location
    // when properties are reviewed, they will go in board order
    // NOTE: GameBoard has an array of 41 spaces, because "In Jail" is a special space that can't be landed on
    std::shared_ptr<BoardSpace> m_propertiesInOrder[40]; 

    // because properties get added to this one at a time, it's actually really easy to keep them in order
    // there's no need for complex sorting algorithms, literally just go through existin properties
    // and when you find a board location that a new one is adjacent to, insert it
    std::vector<std::shared_ptr<BoardSpace>> m_ownedProperties;

    // ownedRails determines the rent for all owned rails
    // technically, the size of railList would do the same job
    std::vector< std::shared_ptr<BoardSpace>> m_railList;
    int m_ownedRails = 0;

    // rent for owning 1 is rollx4, rent for owning 2 is rollx10
    std::vector<std::shared_ptr<BoardSpace>> m_utilsList;
    int m_ownedUtilities = 0;

    void resetPropertiesArray();

public:
    //GameAdmin* admin;
    std::string m_playerName = "";
    int m_currentLocationIndex = 0;
    int m_auctionBid = 0;
    int m_rolledDoubles = 0; // when this hits 3, go to jail, then this reduces by 1 per turn and leave jail at zero (or earlier)

    Player()
    {
        for (int i = 0; i < c_boardSize; i++)
        {
            m_propertiesInOrder[i] = nullptr;
        }
    };
    Player(std::string aName, int aMoney)
    {
        m_playerName = aName;
        m_totalMoney = aMoney;
        
        // initialize properties array
        for (int i = 0; i < c_boardSize; i++)
        {
            m_propertiesInOrder[i] = nullptr;
        }
    }
    ~Player();

    

    bool operator==(const Player& player);
    //void operator+=(const Player& player);

    void setAdmin(std::shared_ptr<GameAdmin> admin);

    int rollResult(int die1, int die2);
    bool tooManyDoubles();
    bool isInJail();
    void setJailStatus(bool newStatus);
    void incrementJailTurns();
    int getJailTurns();
    void resetDoubles();
    void moveTo(int spaceIndex);
    int getRollSum();

    // pay & receive could be a single function that accepts a negative or positive value and always adds it
    // if payRent and payMoney were different functions, a stat could track how much was paid specifically on rent
    // same for receiveRent/receiveMoney. for ease of conversion, could make overloaded function with extra bool isRent = false
    // would only need to change it for paying rent, which only exists in one place, while payMoney exists other places
    void payMoney(int amount);
    void receiveMoney(int amount);
    bool isBankrupt();
    void goBankrupt(std::shared_ptr<Player> debtor); // if player goes bankrupt while owing someone
    void goBankrupt(); // if player goes bankrupt while not owing someone
    int getCurrentFunds();
    bool canAfford(int price);
    void setDebtor(std::shared_ptr<Player> player);

    bool hasCompleteSet(Colour setColour);
    
    void reviewProperties();
    void acquireProperty(std::shared_ptr <BoardSpace> aNewSpace);
    void updateSetSpaces(std::shared_ptr<BoardSpace>& aNewSpace, bool isComplete);
    void organizeNewProperty(std::shared_ptr <BoardSpace> aNewSpace);
    int getPropertyCount();

    void forfeitProperty(std::shared_ptr<BoardSpace> aNewSpace);

    int checkOwnedUtilities();
    void modifiyOwnedUtilities(int val);

    int auctionOptions(int currentHighBid);
    int getAuctionBid();
    void resetAuction();

    bool hasBonusRoll();

    void mortgagedUtility(bool mortgaged);
    void mortgagedRail(bool mortgaged);

    void setJailCards(int newVal);
    int countJailCards();

    tradeOffer updateTrade(tradeOffer t);
    void performTrade(tradeOffer out, tradeOffer in);

    char playerMenu(bool includePass, bool includeTrade, std::shared_ptr<Player> pOwner);
};

