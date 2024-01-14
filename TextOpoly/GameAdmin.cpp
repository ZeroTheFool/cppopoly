#include "gameAdmin.h"
#include "Player.h"
#include "BoardSpace.h"
#include "Utility.h"
#include "Tax.h"
#include "tradeManager.h"
#include "CardSystem.h"
#include <iostream>
#include <vector>
#include <memory>
#include <Windows.h>

using namespace std;

void GameAdmin::makePlayerList()
{
    std::shared_ptr<Player> makePlayer;
    std::string newName;
    m_playerCount = 0;

    cout << "Enter Player Names " << m_playerCount + 1 << " ( 'start' to begin, 'quit' to exit )\n";
    do
    {
        cout << "Player " << m_playerCount + 1 << ":";
        cin >> newName;

        if (newName == "quit") // set players to zero and break loop
        {
            m_playerCount = 0;
            break;
        }

        if (newName != "start")
        {
            makePlayer = make_shared<Player>(newName, 1000);
            m_playerList.push_back(makePlayer);
            m_playerCount++;
        }
    } while (newName != "start");

}

int GameAdmin::getPlayerCount()
{
    return m_playerCount;
}

std::shared_ptr<Player> GameAdmin::newRound()
{
    // in the event that enough players are bankrupt
    // that no one can take a turn and it gets back to the last player to go,
    // the game should end
    int previousPlayer = m_newPlayerIndex;

    m_roundCount++; // TODO: i don't think this does anything anymore

    do
    {
        // increment player index to be next player
        m_newPlayerIndex++;

        // wrap around if goes beyond last player
        if (m_newPlayerIndex == m_playerCount)
            m_newPlayerIndex = 0;
        // if the next player would be bankrupt, go to next next player until a non-bankrupt player is found
    } while (m_playerList.at(m_newPlayerIndex)->isBankrupt() == true);

    // if the next non-bankrupt player is not the player whose turn just ended, return this new player
    if (m_newPlayerIndex != previousPlayer)
    {
        // a new player was found
        m_playerList.at(m_newPlayerIndex)->resetDoubles();
        return m_playerList.at(m_newPlayerIndex);
    }
    else // otherwise, it looped back to the previous player and all other players are bankrupt
    {
        return nullptr;
    }

}

bool GameAdmin::eliminatePlayer()
{
    int currentPlayers = 0;
    for (int i = 0; i < m_playerCount; i++)
    {
        if (!m_playerList.at(i)->isBankrupt())
        {
            currentPlayers++;
        }
    }
    return (currentPlayers > 1); // greater than 1 means more than one non-bankrupt player
}


int GameAdmin::getRoundNum()
{
    return static_cast<int>(m_roundCount / m_playerCount + 1);
}

void GameAdmin::displayRoundHeader()
{
    cout << "Round: " << getRoundNum() << endl;
    cout << "==========================" << endl;
    for (int p = 0; p < m_playerCount; p++)
    {
        if (p == m_newPlayerIndex) // for current player, add an arrow next to name
        {
            cout << "->";
        }
        cout << m_playerList.at(p)->m_playerName << ": $" << m_playerList.at(p)->getCurrentFunds();
        if (p != m_playerCount - 1)
            cout << " | "; // only put a bar after player names that aren't last
    }
    cout << "\n==========================" << endl;
}

int GameAdmin::rollDice(int& die1, int& die2)
{
    die1 = rand() % 6 + 1;
    die2 = rand() % 6 + 1;
    std::cout << "Rolled " << (die1 + die2) << " (" << die1 << "," << die2 << ")" << endl;
    return die1 + die2;
}

bool GameAdmin::endGame()
{
    // display winner
    // this state can only be reached if all but one player is bankrupt, so find that one
    for (int i = 0; i < m_playerCount; i++)
    {
        if (!m_playerList.at(i)->isBankrupt())
        {
            cout << m_playerList.at(i)->m_playerName << " is the Winner!" << endl;
            std::cout << "GAME ENDED" << endl;
            system("pause");
            return true;
        }
    }
    return false;
}

int GameAdmin::getCurrentPlayerIndex()
{
    return m_newPlayerIndex;
}

std::shared_ptr<Player> GameAdmin::getPLayerFromName(std::string aName)
{
    for (int i = 0; i < m_playerCount; i++)
    {
        if (m_playerList.at(i)->m_playerName == aName)
            return m_playerList.at(i);
    }
    return nullptr;
}

std::vector<std::shared_ptr<Player>> GameAdmin::getPlayerList()
{
    return m_playerList;
}

void GameAdmin::updateSetStatus(int boardIndex, bool isComplete)
{
    m_board->getCurrentSpace(boardIndex)->updateSetStatus(isComplete);
}

int GameAdmin::getJailFee()
{
    return c_jailFee;
}

int GameAdmin::getHousePool()
{
    return m_housePool;
}

int GameAdmin::getHotelPool()
{
    return m_hotelPool;
}

void GameAdmin::setHousePool(int newVal)
{
    m_housePool = newVal;
}

void GameAdmin::setHotelPool(int newVal)
{
    m_hotelPool = newVal;
}


void GameAdmin::auctionProperty(std::shared_ptr<BoardSpace> cProperty)
{
    int playerIndex = m_newPlayerIndex;
    int leadingBid = 0;
    int incomingBid = 0;
    int passedPlayers = 0;
    std::string leaderName = "";
    std::shared_ptr<Player> currentLeader = nullptr;
    std::shared_ptr<Player> currentBidder = m_playerList.at(playerIndex);
    do
    {
        system("cls");

        cout << "=====AUCTION=====" << endl;
        cout << "Current bid: $" << leadingBid;
        if (currentLeader != nullptr)
        {
            cout << "(" << currentLeader->m_playerName << ")" << endl;
        }
        else
        {
            cout << endl;
        }
        cout << "\n";
        cProperty->displayDetails();
        if (currentLeader == currentBidder) // somehow the highet bidder is now bidding again, break so he wins
            break;

        // bankrupt players are not eligible to auction
        if (!currentBidder->isBankrupt())
        {
            // if a player has already passed, skip them. also skip bankrupt 
            if (currentBidder->getAuctionBid() != -1)
            {
                // send current high bid to player class, get back either -1 (withdraw) or a new higher value
                incomingBid = currentBidder->auctionOptions(leadingBid);
                // returning -1 means they withdrew
                if (incomingBid == -1)
                {
                    // track how many players have passed
                    passedPlayers++;
                    // reset passed player's bid to 0, only needed to be -1 for this specific check

                }
                else
                {
                    // don't need to check if incoming is winning, the bidding player is only able to return -1 or a winning number
                    leadingBid = incomingBid;
                    currentLeader = currentBidder;
                }
            }
            else
            {
                // remind player they are being skipped
                cout << currentBidder->m_playerName << " already withdrew" << endl;
            }
        }
        Sleep(1000);
        playerIndex = (playerIndex < (m_playerCount - 1) ? playerIndex + 1 : 0); // go to next player in order
    } while (passedPlayers < (m_playerCount - 1)); // when all but one player have passed, the remaining player wins

    if (currentLeader != nullptr)
    {
        leaderName = currentLeader->m_playerName;
        //printf("\n%s wins the auction for $%i",leaderName,leadingBid);
        cout << "\n" << currentLeader->m_playerName << " wins the auction for $" << leadingBid << endl;
        currentLeader->acquireProperty(cProperty);
        currentLeader->payMoney(leadingBid);
        Sleep(2500);
    }
    else
    {
        //printf("Error");
        cout << "error" << endl;
    }

    // reset everyone's bid when auction ends
    for (int i = 0; i < m_playerCount; i++)
    {
        m_playerList.at(i)->resetAuction();
    }
}

void GameAdmin::handleTrade(int receiverIndex)
{
    // gameAdmin knows who the current player is, don't need to pass it through
    std::shared_ptr<Player> trader = m_playerList.at(m_newPlayerIndex);
    std::shared_ptr<Player> receiver = m_playerList.at(receiverIndex);

    // make array of the two traders so trade initiator is index 0 and the other is index 1
    std::shared_ptr<Player> traders[2] = { trader, receiver };

    // initialize a trade for each player
    tradeOffer tradeOut;
    tradeOut.tradeMoney = 0;
    tradeOut.tradeJailCards = 0;
    tradeOut.tradeProperties;
    tradeOut.trader = trader; // tradeOut is from the initiator to receiver

    tradeOffer tradeIn;
    tradeIn.tradeMoney = 0;
    tradeIn.tradeJailCards = 0;
    tradeIn.tradeProperties;
    tradeIn.trader = receiver; // tradeIn is from the receiver to initiator

    // store both trades in an array, the indexes match the traders array
    tradeOffer tradeList[2] = { tradeOut, tradeIn };

    int menuSelection = 0; // user friendly input #
    int tradeIndex = 0; // menuSelection - 1

    int currentTrader = 0; // this will track who is in charge of trading
    bool traderAccept[2] = { false, false };

    do
    {
        cout << traders[currentTrader]->m_playerName << " deciding..." << endl;
        // prompt user: modify tradeOut or tradeIn?
        // make it more user friendly
        do
        {
            cout << "Modify trades" << endl;
            cout << "1 - tradeOut" << endl;
            cout << "2 - tradeIn" << endl;
            cout << "3 - accept" << endl;
            cin >> menuSelection;

        } while (menuSelection < 1 && menuSelection > 3); // a little error checking never hurts

        tradeIndex = menuSelection - 1;

        // based on input, pass whoever the current trader whichever trade they picked
        if (menuSelection != 3)
        {
            tradeList[tradeIndex] = traders[currentTrader]->updateTrade(tradeList[tradeIndex]);
            // if the trade is modified, both players have to unaccept
            // current trader will accept when they are done, other playe can then modify or accept
            traderAccept[0] = false;
            traderAccept[1] = false;
        }
        else
        {
            // acccept the trade and swap to other player
            traderAccept[currentTrader] = true;
            currentTrader = (currentTrader == 0 ? 1 : 0);
        }
    } while (traderAccept[0] == false || traderAccept[1] == false); // keep looping as long as one party does not agree

    // actually do the trade
    // swap structs between players
    // the function specifically takes the out trade and then the in trade
    // player 0 loses 0 and gains 1
    // player 1 loses 1 and gains 0
    traders[0]->performTrade(tradeList[0], tradeList[1]);
    traders[1]->performTrade(tradeList[1], tradeList[0]);

    // reset trade bools
    // don't need to reset structs, they go out of scope when this function ends anyway
    for (int t = 0; t < 2; t++) // go through both trade structs
    {
        for (int p = 0; p < tradeList[t].tradeProperties.size(); p++) // go through each property in the trade struct
        {
            // these are only in this list because they were flagged for trade,
            // so just unflag them and they won't show as trade listed on the board
            tradeList[t].tradeProperties[p]->updateTradeList();
        }

    }

}

PropertyResult GameAdmin::handleProperty(std::shared_ptr<BoardSpace> cProperty)
{
    std::shared_ptr<Player> activePlayer = m_playerList.at(m_newPlayerIndex);
    char menuSelection;

    // first check who owns property, if self or not
    std::string owner = cProperty->getOwner();

    //Player* owner = cProperty->getOwner(); // i would really like this to be stored as an object

    if (owner == "None")
    {
        cout << "No one owns this yet" << endl;
        // prompt to buy
        int purchasePrice = cProperty->getPurchasePrice();

        if (activePlayer->canAfford(purchasePrice))
        {
            cout << "Buy for $" << purchasePrice << " (y/n)?" << endl;
            cin >> menuSelection;

            // loop for error checking
            while (menuSelection != 'n' && menuSelection != 'N' && menuSelection != 'y' && menuSelection != 'Y')
            {
                cout << "Invalid entry." << endl;
                cout << "Buy for $" << purchasePrice << " (y/n)?" << endl;
                cin >> menuSelection;
            }

            switch (menuSelection)
            {
            case 'y':
            case 'Y':
                activePlayer->payMoney(purchasePrice);
                activePlayer->acquireProperty(cProperty);
                cProperty->setOwner(activePlayer->m_playerName);
                return PropertyResult::BoughtNew;

            case 'n':
            case 'N':
                return PropertyResult::AuctionNeeded;
            }
        }
        else
        {
            cout << "You can't afford this property. An auction will be held." << endl;
            return PropertyResult::AuctionNeeded;
        }
    }

    else if (owner == activePlayer->m_playerName)
    {
        cout << "You own this" << endl;
        return PropertyResult::None;
    }

    else if (owner != "BANK") // BANK owns spaces that cannot be purchased, so this check means someone owns it
    {
        cout << owner << " owns this" << endl;
        std::shared_ptr<Player> pOwner = getPLayerFromName(owner);


        // mortgaged properties don't collect rent
        if (cProperty->checkMortgaged())
        {
            cout << "Property is mortgaged. No rent is due" << endl;
            return PropertyResult::None;
        }
        else
        {
            activePlayer->setDebtor(pOwner);
            int rent = 0;
            if (dynamic_pointer_cast<Utility>(cProperty))
            {
                rent = cProperty->getRentFee(activePlayer->getRollSum());
            }
            else
            {
                rent = cProperty->getRentFee();
            }

            // the original loop has to return the outcome of the transaction
            // so hopefully i can just return what gets returned
            return promptPlayerToPay(rent, pOwner); // TODO: fix this, put this function on Player
        }
    }
    else if (owner == "BANK") // this is a tax or a corner
    {
        int rent = 0;
        // determine if Tax -> get rent fee (tax amount)
        // determine if Utility -> get rent fee based on roll
        if (dynamic_pointer_cast<Tax>(cProperty))
        {
            rent = cProperty->getRentFee();
        }

        if (rent != 0) // rent charged because this is a tax space
        {
            //promptPlayerToPay(activePlayer, rent, nullptr); TODO: fix this, too
        }
    }
    return PropertyResult::None;
}


PropertyResult GameAdmin::promptPlayerToPay(int rent, std::shared_ptr<Player> pOwner)
{
    // get current player
    std::shared_ptr<Player> cPlayer = m_playerList.at(m_newPlayerIndex);
    char menuSelection = 'p';

    do
    {
        if (cPlayer->canAfford(rent))
        {
            cPlayer->payMoney(rent);
            if (pOwner)
            {
                cout << cPlayer->m_playerName << " paid $" << rent << " to " << pOwner->m_playerName << endl;
                pOwner->receiveMoney(rent);
            }
            else
            {
                cout << cPlayer->m_playerName << " paid $" << rent << " to the Bank" << endl;
            }

            return PropertyResult::PaidRent;
        }
        else
        {
            cout << "You can't afford this amount: $" << rent << endl;

            // prompt player to review properties to mortgage or go bankrupt
            menuSelection = cPlayer->playerMenu(false, false, pOwner);

            if (menuSelection == 'Q') // already fully confirmed in the menu function
            {
                return PropertyResult::Bankruptcy;
            }
        }
    } while (!cPlayer->canAfford(rent));

    // should never reach this point
    return PropertyResult::None;
}


//************************************************
//              CARD FUNCTIONS
//************************************************

PropertyResult GameAdmin::cardActionMenu(GameCard* newCard)
{
    switch (newCard->m_type)
    {
    case functionType::Pay:
        //return cardPayMoney(newCard);
        break;
    case functionType::Receive:
        break;

    case functionType::MoveToIndex:

        break;

    case functionType::GoToJail:
        cardGoToJail();
        break;

    case functionType::GainJailCard:
        cardGainJailCard();
        break;

    case functionType::MoveAndPay:
        break;

    }
    // if an above case doesn't return first, return none
    return PropertyResult::None;
}
//
//PropertyResult GameAdmin::cardPayMoney(std::shared_ptr<Card> newCard)
//{
//    // determine who to pay
//    // either Bank or All
//    std::shared_ptr<Player> activePlayer = m_playerList.at(m_newPlayerIndex);
//
//    char menuSelection = 'q';
//    int payTotal = newCard->m_functionValue; // save the original value to pay all
//    if (newCard->m_target == functionTarget::All)
//    {
//        payTotal = newCard->m_functionValue * getPlayerCount(); // multiply by number of players
//    }
//    while (!activePlayer->canAfford(payTotal) && menuSelection != 'Q') // check player can pay the total, whether multiplied or not
//    {
//        menuSelection = activePlayer->playerMenu(false, false, nullptr);
//    }
//
//    // exiting the loop means they can afford to pay whatever the amount was, or went bankrupt
//    if (menuSelection != 'Q')
//    {
//        activePlayer->payMoney(payTotal);
//        if (newCard->m_target == functionTarget::All)
//        {
//            for (int i = 0; i < m_playerCount; i++)
//            {
//                // each player gets the original, unmodiifed, value
//                m_playerList.at(i)->receiveMoney(newCard->m_functionValue);
//            }
//        }
//        return PropertyResult::PaidRent;
//    }
//    else
//    {
//        // chose bankruptcy
//        // this will ensure their turn is ended back in main
//        return PropertyResult::Bankruptcy;
//    }
//    return PropertyResult::None;
//}
//
//PropertyResult GameAdmin::cardMoveToSpace(std::shared_ptr<Card> newCard)
//{
//    std::shared_ptr<Player> activePlayer = m_playerList.at(m_newPlayerIndex);
//
//    // check that player isn't on Go or Jail, that means a property someone may own
//    if (newCard->m_functionValue != 0 && newCard->m_functionValue != 40)
//    {
//        // first check if player will pass Go by moving
//        if (activePlayer->m_currentLocationIndex > newCard->m_functionValue)
//        {
//            // will pass go, because you can't move backwards
//            // collect $200
//        }
//
//        // move player, get new space
//        activePlayer->moveTo(newCard->m_functionValue);
//        std::shared_ptr<BoardSpace> currentLocation = m_board.getCurrentSpace(activePlayer->m_currentLocationIndex);
//        
//        // show details for user-friendliness
//        currentLocation->printName();
//        currentLocation->displayDetails();
//
//        // resolve the new property
//        // need to tell property if an extra fee is required to be paid
//        return handleProperty(currentLocation);
//    }
//
//    return PropertyResult::None;
//}

void GameAdmin::cardGoToJail()
{
    std::shared_ptr<Player> activePlayer = m_playerList.at(m_newPlayerIndex);
    activePlayer->setJailStatus(true);
}

void GameAdmin::cardGainJailCard()
{
    std::shared_ptr<Player> activePlayer = m_playerList.at(m_newPlayerIndex);
    activePlayer->setJailCards(1);
}
