#pragma once
#include "Player.h"
#include "GameBoard.h"
#include "BoardSpace.h"
//#include "tradeManager.h"
//#include "CardSystem.h"
#include "GameCard.h"
#include "enum.h"

#include <vector>
#include <map>

//class Player;
//class GameBoard;
//class BoardSpace;
class tradeManager;

class GameAdmin
{
private:
    int m_roundCount = 0;
    int m_newPlayerIndex = -1; // used to get the next player in order
    int m_playerCount = 0;
    int m_maxPlayerCount = 4;
    std::vector<std::shared_ptr<Player>> m_playerList;

    GameBoard* m_board;

    const int c_jailFee = 50;

    // before a house/hotel can be built, need to make sure one is available
    int m_housePool = 50;
    int m_hotelPool = 20;

public:
    //GameAdmin() {};
    GameAdmin(GameBoard* board)
    {
        m_board = board;
        m_roundCount = 0;
        m_newPlayerIndex = -1;
        makePlayerList();
    }

    void makePlayerList();
    int getPlayerCount();


    std::shared_ptr<Player> newRound(); // increment round count, return new player, also call displayRoundHeader

    bool eliminatePlayer();

    int getRoundNum();
    void displayRoundHeader();
    int rollDice(int& die1, int& die2); // roll two 6-sided dice, return the sum

    bool endGame();

    int getCurrentPlayerIndex();

    std::shared_ptr<Player> getPLayerFromName(std::string aName);
    std::vector<std::shared_ptr<Player>> getPlayerList();

    void updateSetStatus(int boardIndex, bool isComplete);

    int getJailFee();

    int getHousePool();
    int getHotelPool();
    void setHousePool(int newVal);
    void setHotelPool(int newVal);

    void auctionProperty(std::shared_ptr<BoardSpace> cProperty);
    void handleTrade(int receiverIndex);
    PropertyResult handleProperty(std::shared_ptr<BoardSpace> cProperty);
    PropertyResult promptPlayerToPay(int rent, std::shared_ptr<Player> pOwner);


    // CARD FUNCTIONS
    PropertyResult cardActionMenu(GameCard* newCard);

    std::string formatDisplayNameRow(std::string text, int width);

    ////functionType::Pay
    PropertyResult cardPayMoney(GameCard* newCard);

    ////functionType::Receive
    void cardReceiveMoney(GameCard* newCard); /* check payer, etc */

    ////functionType::MoveToIndex
    //PropertyResult cardMoveToSpace(std::shared_ptr<Card> newCard);
    void cardGoToJail();

    ////functionType::MoveAndPay
    ////functionType::MoveAndPayExtra only diff is the multiplier, if it's default x1 then it can just always be included
    //int moveAndPay(std::shared_ptr<Player> player, std::shared_ptr<Card> newCard);

    ////functionType::GainJailCard
    void cardGainJailCard();
};


