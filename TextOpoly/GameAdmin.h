#pragma once
#include "Player.h"
#include "GameBoard.h"
#include "BoardSpace.h"
#include "tradeManager.h"
//#include "CardSystem.h"
#include "enum.h"

#include <vector>
#include <map>

class GameBoard;

class GameAdmin
{
private:
    int m_roundCount = 0;
    int m_newPlayerIndex = -1; // used to get the next player in order
    int m_playerCount = 0;
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

    int getJailFee();

    int getHousePool();
    int getHotelPool();
    void setHousePool(int newVal);
    void setHotelPool(int newVal);

    void auctionProperty(std::shared_ptr<BoardSpace> cProperty);
    void handleTrade(std::shared_ptr<Player> trader, std::shared_ptr<Player> receiver);
    PropertyResult handleProperty(std::shared_ptr<BoardSpace> cProperty);


    // CARD FUNCTIONS
    //PropertyResult cardActionMenu(std::shared_ptr<Card> newCard);

    ////functionType::Pay
    //PropertyResult cardPayMoney(std::shared_ptr<Card> newCard);

    ////functionType::Receive
    //int receiveMoney(std::shared_ptr<Player> player, std::shared_ptr<Card> newCard); /* check payer, etc */

    ////functionType::MoveToIndex
    //PropertyResult cardMoveToSpace(std::shared_ptr<Card> newCard);
    void cardGoToJail();

    ////functionType::MoveAndPay
    ////functionType::MoveAndPayExtra only diff is the multiplier, if it's default x1 then it can just always be included
    //int moveAndPay(std::shared_ptr<Player> player, std::shared_ptr<Card> newCard);

    ////functionType::GainJailCard
    void cardGainJailCard();
};


