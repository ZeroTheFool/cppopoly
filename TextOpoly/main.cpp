#include "Player.h"
#include "GameAdmin.h"
#include "BoardSpace.h"
#include "Property.h"
#include "Railroad.h"
#include "Corner.h"
#include "Tax.h"
#include "CardSpace.h"
#include "GameBoard.h"
#include "tradeManager.h"

#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <memory>

using namespace std;





char playerMenu(bool includePass, bool includeTrade, std::shared_ptr<Player> cPlayer, std::shared_ptr<GameAdmin> admin, std::shared_ptr<Player> pOwner);
PropertyResult handleProperty(std::shared_ptr<BoardSpace> cProperty, std::shared_ptr<Player> cPlayer, std::shared_ptr<GameAdmin> admin);
void displayCurrentSpace(std::shared_ptr<BoardSpace> cBoardSpace);
PropertyResult promptPlayerToPay(std::shared_ptr<Player> cPlayer, int rent, std::shared_ptr<GameAdmin> admin, std::shared_ptr<Player> pOwner);

int main()
{
    srand((unsigned int)time(NULL));

    GameBoard board;

    std::shared_ptr<BoardSpace> currentLocation;

    cout << "Welcome to" << endl;
    cout << "        ____          ____   ____   ____\n|\\  /| |    | |\\   | |    | |    | |    | |      \\   /\n| \\/ | |    | | \\  | |    | |____| |    | |       \\ /\n|    | |    | |  \\ | |    | |      |    | |        |\n|    | |____| |   \\| |____| |      |____| |_____   |\n" << endl;
    
    std::shared_ptr<GameAdmin> admin = make_shared<GameAdmin>(&board);
    int playerCount = admin->getPlayerCount();

    for (int i = 0; i < playerCount; i++)
    {
        admin->getPlayerList().at(i)->setAdmin(admin);
    }

    // if no player names are entered, can't start game so just exit
    // needs a real quit message, and also needs to delete pointers


    int roundCount = 0;

    std::shared_ptr<Player> currentPlayer = nullptr;

    int dieRollOne = 0;
    int dieRollTwo = 0;
    int dieRollSum = 0;

    //TESTING: give player a property
    //playerList.at(0)->acquireProperty(board.propertyList.at(0));

    int currentLocationIndex;;
    char menuSelection = 'p'; // default pass turn


    // *****************************
    // MAIN GAME LOOP
    // *****************************

    do // TESTING: for 20 turns or x is entered
    {
        // *****************************
        // ADVANCE ROUND IF CURRENT PLAYER IS DONE
        // *****************************
        dieRollSum = 0;
        // if current player had rolled doubles, they get a bonus roll
        // otherwise advance to the next player
        if (!currentPlayer || !currentPlayer->hasBonusRoll())
        {
            currentPlayer = admin->newRound();
            if (currentPlayer == nullptr)
            {
                // this happens when there are no non-bankrupt players to advance to
                if (admin->endGame())
                    return 0;
            }
            cout << "Start Turn" << endl;
        }
        else // current player did roll doubles, gets to roll again
        {
            cout << "Roll Again" << endl;
        }

        // prompt user to start turn so it doesn't feel uncontrolled
        system("pause");

        system("cls");
        admin->displayRoundHeader();

        // *****************************
        // ROLL DICE or RESOLVE JAIL
        // *****************************
        if (currentPlayer->isInJail())
        {
            displayCurrentSpace(board.getCurrentSpace(currentPlayer->m_currentLocationIndex)); // this will be location 40 "In Jail"

            // jail menu
            do
            {
                cout << "Jail Options" << endl;
                if (dieRollSum == 0)
                {
                    cout << "1 - Roll Doubles" << endl;
                }
                else
                {
                    cout << "Already rolled" << endl;
                }
                if (currentPlayer->canAfford(admin->getJailFee()))
                {
                    cout << "2 - Pay Fine ($" << admin->getJailFee() << ")" << endl;
                }
                else
                {
                    cout << "Can't afford fine ($" << admin->getJailFee() << ")" << endl;
                }
                if (currentPlayer->countJailCards() > 0)
                {
                    cout << "3 - Use Get Out of Jail Free (" << currentPlayer->countJailCards() << ")" << endl;
                }
                cout << "0 - Pass" << endl;
                cin >> menuSelection;

                switch (menuSelection)
                {
                case 0:
                    break;
                case 1:
                    // need to roll doubles to escape jail
                    if (dieRollSum == 0) // hasn't rolled yet
                    {
                        dieRollSum = admin->rollDice(dieRollOne, dieRollTwo);
                        if (dieRollOne == dieRollTwo) // rolled doubles
                        {
                            // escaped jail
                            currentPlayer->setJailStatus(false);
                            cout << "Escaped jail" << endl;
                        }
                        else
                        {
                            cout << "Failed to roll doubles" << endl;
                        }
                        break;
                    }
                case 2:
                    // pay fine to leave jail
                    if (currentPlayer->canAfford(admin->getJailFee()))
                    {
                        currentPlayer->payMoney(admin->getJailFee());
                        currentPlayer->setJailStatus(false);
                        cout << "Escaped jail" << endl;
                    }
                    break;
                case 3:
                    // spend a jail card
                    if (currentPlayer->countJailCards() > 0)
                    {
                        currentPlayer->setJailCards(-1);
                        currentPlayer->setJailStatus(false);
                        cout << "Escaped jail" << endl;
                    }
                    break;

                }
                // loop until player is no longer in jail, or they choose to stay
            } while (menuSelection != 0 && currentPlayer->isInJail());

            // if still in jail, skip remainder of turn
            if (currentPlayer->isInJail())
                continue;

        } // handled being in jail
        else
        {
            displayCurrentSpace(board.getCurrentSpace(currentPlayer->m_currentLocationIndex));
            // were not already in jail, need a die roll
            cout << "Rolling Dice for " << currentPlayer->m_playerName << endl;
            Sleep(1500);

            if (dieRollSum == 0) // need to roll, this won't be 0 if the player rolled to escape jail
            {
                dieRollSum = admin->rollDice(dieRollOne, dieRollTwo);
            }

            // *****************************
            // CHECK ROLL FOR DOUBLES
            // *****************************

            if (dieRollOne == dieRollTwo)
            {
                cout << currentPlayer->m_playerName << " rolled Doubles" << endl;

                if (currentPlayer->tooManyDoubles())
                {
                    cout << "Go to Jail" << endl;
                    currentPlayer->setJailStatus(true);
                    continue; // skip remainder of turn
                }
            }
            else
            {
                // whether this is their first roll or third, not rolling doubles resets their doubles counter
                currentPlayer->resetDoubles();
            }

        } // rolled for not in jail

        // *****************************
        // GET NEW LOCATION FROM ROLL
        // *****************************

        // whether or not this player started in jail and escaped, or did not start in jail,
        // use their roll (from jail or not) to move to new space

        currentLocationIndex = currentPlayer->rollResult(dieRollOne, dieRollTwo);
        cout << "Moving..." << endl;

        if (currentLocationIndex > 39)
        {
            // passed GO, collect $200
            cout << currentPlayer->m_playerName << " passed GO. Collect $200" << endl;
            currentPlayer->receiveMoney(200);
            // set location index to a valid number
            currentLocationIndex -= 40; //TESTING: movement
        }
        //cout << "[" << currentLocationIndex << "]" << endl; //TESTING: just shows the current space index
        currentLocation = board.getCurrentSpace(currentLocationIndex);

        Sleep(1500);

        system("cls");
        admin->displayRoundHeader();

        // *****************************
        // SHOW LOCATION HEADER AND HANDLE LOCATION
        // *****************************
        cout << "[Rolled: " << currentPlayer->getRollSum() << "] ";
        displayCurrentSpace(currentLocation); // title box

        currentLocation->displayDetails();

        PropertyResult result = PropertyResult::None;

        // check if this new space is a Corner or not
        if (!dynamic_pointer_cast<Corner> (currentLocation))
        {
            if (dynamic_pointer_cast<CardSpace> (currentLocation))
            {
                // don't need to do any property stuff for a cardspace
                dynamic_pointer_cast<CardSpace> (currentLocation)->drawNextCard();
            }
            else
            {
                // first resolve the new space: unowned -> buy, owned -> nothing, rent due -> pay
                result = handleProperty(currentLocation, currentPlayer, admin);

            }
        }
        else
        {
            // corner space: jail
            if (currentLocation->checkIsSendJail()) // if true, is jail
            {
                cout << currentPlayer->m_playerName << " has been sent to Jail" << endl;
                currentPlayer->setJailStatus(true);
            }
            // corner space: not jail
            // 
        }

        Sleep(2000);

        switch (result)
        {
        case PropertyResult::AuctionNeeded:
            // do auction stuff -- this function call could be inside handle property
            admin->auctionProperty(currentLocation);
            break;
        case PropertyResult::Bankruptcy:
            if (!admin->eliminatePlayer()) // false means only one player not bankrupt, game ends
            {
                if (admin->endGame())
                    return 0;
            }
            continue;
        case PropertyResult::SentToJail:
            //
            continue;
        }

        // *****************************
        // CONTINUE PLAYER TURN
        // *****************************

        do
        {
            system("cls");
            admin->displayRoundHeader();
            cout << "[Rolled: " << currentPlayer->getRollSum() << "] ";
            displayCurrentSpace(currentLocation);
            currentLocation->displayDetails();

            cout << "\n====================" << endl;

            menuSelection = currentPlayer->playerMenu(true, true, nullptr);

        } while (menuSelection != 'p' && menuSelection != 'x' && menuSelection != 'Q');

        if (menuSelection == 'Q')
        {
            if (!admin->eliminatePlayer()) // false means only one player not bankrupt, game ends
            {
                if (admin->endGame())
                    return 0;
            }
            continue; // skips remainder of this player's turn, but this is the end of the loop anyway
        }

        system("cls");

    } while (menuSelection != 'x');

    cout << "Game ended" << endl;

    return 0;
}


char playerMenu(bool includePass, bool includeTrade, std::shared_ptr<Player> cPlayer, std::shared_ptr<GameAdmin> admin, std::shared_ptr<Player> pOwner)
{
    char menuSelection = 'r';

    if (cPlayer->getPropertyCount() > 0)
        cout << "r - review properties" << endl;

    if (includeTrade)
        cout << "t - trade properties" << endl;

    if (includePass)
        cout << "p - pass turn" << endl;

    cout << "Q - declare bankruptcy" << endl;

    cin >> menuSelection;

    if (menuSelection == 'r')
    {
        if (cPlayer->getPropertyCount() > 0)
        {
            cPlayer->reviewProperties();

        }
    }
    else if (menuSelection == 't' && includeTrade)
    {
        int chosenPartner = -1;
        // prompt user to choose a player to trade with
        do
        {
            cout << "Who To Trade With:" << endl;
            for (int i = 0; i < admin->getPlayerCount(); i++)
            {
                if (i != admin->getCurrentPlayerIndex())
                {
                    // only print names and user-friendly indexes of other players
                    //printf("%i - %s", i + 1, admin->getPlayerList().at(i));
                    cout << i + 1 << " " << admin->getPlayerList().at(i) << endl;
                }
            }
            cin >> chosenPartner;
            chosenPartner--;

            // loop until they don't choose themself
        } while (chosenPartner == admin->getCurrentPlayerIndex());

        std::shared_ptr<Player> tradePartner = admin->getPlayerList().at(chosenPartner);
        admin->handleTrade(cPlayer, tradePartner);
    }

    else if (menuSelection == 'Q')
    {
        cout << "Q - confirm bankruptcy" << endl;
        cin >> menuSelection;
        if (menuSelection == 'Q')
        {
            if (pOwner)
            {
                cPlayer->goBankrupt(pOwner);
            }
            else
            {
                cPlayer->goBankrupt();
            }
        }
    }
    else if (menuSelection == 'p' && includePass)
    {
        return menuSelection;
    }
    else
    {
        cout << "Invalid entry" << endl;
    }

    return menuSelection; // unless this is Q, it doesn't matter
}

PropertyResult handleProperty(std::shared_ptr<BoardSpace> cProperty, std::shared_ptr<Player> cPlayer, std::shared_ptr<GameAdmin> admin)
{
    char menuSelection;

    // first check who owns property, if self or not
    std::string owner = cProperty->getOwner();

    //Player* owner = cProperty->getOwner(); // i would really like this to be stored as an object

    if (owner == "None")
    {
        cout << "No one owns this yet" << endl;
        // prompt to buy
        int purchasePrice = cProperty->getPurchasePrice();

        if (cPlayer->canAfford(purchasePrice))
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
                cPlayer->payMoney(purchasePrice);
                cPlayer->acquireProperty(cProperty);
                cProperty->setOwner(cPlayer->m_playerName);
                return PropertyResult::BoughtNew;

            case 'n':
            case 'N':
                //auctionProperty(cProperty, cPlayer);
                return PropertyResult::AuctionNeeded;
            }
        }
        else
        {
            cout << "You can't afford this property. An auction will be held." << endl;
            return PropertyResult::AuctionNeeded;
        }
    }

    else if (owner == cPlayer->m_playerName)
    {
        cout << "You own this" << endl;
        return PropertyResult::None;
    }

    else if (owner != "BANK") // BANK owns spaces that cannot be purchased, so this check means someone owns it
    {
        cout << owner << " owns this" << endl;
        std::shared_ptr<Player> pOwner = admin->getPLayerFromName(owner);


        // mortgaged properties don't collect rent
        if (cProperty->checkMortgaged())
        {
            cout << "Property is mortgaged. No rent is due" << endl;
            return PropertyResult::None;
        }
        else
        {
            cPlayer->setDebtor(pOwner);
            int rent = 0;
            if (dynamic_pointer_cast<Utility>(cProperty))
            {
                rent = cProperty->getRentFee(cPlayer->getRollSum());
            }
            else
            {
                rent = cProperty->getRentFee();
            }

            // the original loop has to return the outcome of the transaction
            // so hopefully i can just return what gets returned
            return promptPlayerToPay(cPlayer, rent, admin, pOwner);
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
            promptPlayerToPay(cPlayer, rent, admin, nullptr);
        }
    }
    return PropertyResult::None;
}

PropertyResult promptPlayerToPay(std::shared_ptr<Player> cPlayer, int rent, std::shared_ptr<GameAdmin> admin, std::shared_ptr<Player> pOwner)
{
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

void displayCurrentSpace(std::shared_ptr<BoardSpace> cBoardSpace)
{
    cout << "You are here:" << endl;
    cBoardSpace->printName();
}
