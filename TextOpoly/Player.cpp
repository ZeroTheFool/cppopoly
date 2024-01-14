#include "Player.h"
#include "GameAdmin.h"
#include "Railroad.h"
#include "Utility.h"
#include "tradeManager.h"
#include <iostream>
#include <vector>
#include <Windows.h>

using namespace std;

void Player::resetPropertiesArray()
{
    for (int i = 0; i < 40; i++)
    {
        m_propertiesInOrder[i] = nullptr;
    }
}


Player::~Player()
{
    resetPropertiesArray();
}

bool Player::operator==(const Player& player)
{
    // i think this is all that would be required
    return (this->m_playerName == player.m_playerName);
}

//void Player::operator+=(const Player& player)
//{
//    // i don't even know if this is worth having
//    // it's the only way i would think you would need to add two Player objects together, if at all
//    this->m_totalMoney += player.m_totalMoney;
//    for (int i = 0; i < 40; i++)
//    {
//        if (player.m_propertiesInOrder[i] != nullptr) // this means they own it, don't need to check owner name
//            this->acquireProperty(player.m_propertiesInOrder[i]);
//    }
//    this->setJailCards(player.m_jailCards);
//}

void Player::setAdmin(std::shared_ptr<GameAdmin> admin)
{
    m_admin = admin;
}

int Player::rollResult(int die1, int die2)
{
    int prevLocation = m_currentLocationIndex;
    m_dieRoll1 = die1;
    m_dieRoll2 = die2;
    m_currentLocationIndex += (die1 + die2);
    if (m_currentLocationIndex > 39)
        m_currentLocationIndex -= 40;

    // the player knows they passed Go and is on the right index
    // return the wrong value so main can process it
    return prevLocation + (die1 + die2);
}

bool Player::tooManyDoubles()
{
    m_rolledDoubles++;
    m_hasBonusRoll = true;
    if (m_rolledDoubles == 3) // rolling doubles three times sends you to jail
    {
        return true;
    }
    return false;
}

bool Player::isInJail()
{
    return m_isInJail;
}

void Player::setJailStatus(bool newStatus)
{
    m_isInJail = newStatus;
    if (m_isInJail)
    {
        m_currentLocationIndex = 40; // location 40 is a special space that cannot be landed on
    }
    else
    {
        m_currentLocationIndex = 9; // location 9 is just visiting
    }
    m_turnsInJail = 0; // whether entering or exiting jail, this should reset to 0, because every roll attempt increases it by 1
}

void Player::incrementJailTurns()
{
    //this will never reach 4, don't need error-checking
    m_turnsInJail++;
}

int Player::getJailTurns()
{
    return m_turnsInJail;
}

void Player::resetDoubles()
{
    m_rolledDoubles = 0;
    m_hasBonusRoll = false;
}

void Player::moveTo(int spaceIndex)
{
    m_currentLocationIndex = spaceIndex;
    // should this also interact with the new space?
    // is there ever a reason it would not?
    // NOTE: being sent to jail is different, it doesn't use this function
}

int Player::getRollSum()
{
    return m_dieRoll1 + m_dieRoll2;
}

void Player::payMoney(int amount)
{
    m_totalMoney -= amount;
}

void Player::receiveMoney(int amount)
{
    m_totalMoney += amount;
}

bool Player::isBankrupt()
{
    return m_isBankrupt;
}

void Player::goBankrupt(std::shared_ptr<Player> debtor)
{
    m_isBankrupt = true;
    debtor->receiveMoney(m_totalMoney);
    m_totalMoney = 0;

    for (int i = 0; i < m_propertyCounter; i++)
    {
        debtor->acquireProperty(m_ownedProperties.at(i));
        //m_propertiesInOrder[i]->setOwner(debtor->m_playerName); // this happes during acquisition by the other player
    }
    m_ownedProperties.clear();
    resetDoubles();

    cout << m_playerName << " declared bankruptcy" << endl;
    cout << "All properties and money have been transferred to " << debtor->m_playerName << endl;
}

void Player::goBankrupt()
{
    m_isBankrupt = true;
    m_totalMoney = 0;

    for (int i = 0; i < m_propertyCounter; i++)
    {
        m_ownedProperties.at(i)->setOwner("BANK"); // sell all properties to the bank. this is not a thing in monopoly
    }
    m_ownedProperties.clear(); // empty the vector
    resetDoubles();

    cout << m_playerName << " declared bankruptcy" << endl;
}

int Player::getCurrentFunds()
{
    return m_totalMoney;
}

bool Player::canAfford(int price)
{
    return (m_totalMoney >= price);
}

void Player::setDebtor(std::shared_ptr<Player> player)
{
    m_inDebtToPlayer = player;
}

bool Player::hasCompleteSet(Colour setColour)
{
    // returns true of the player has the full set of a specified colour
    // note that brown and dark blue only require 2 instead of 3
    switch (setColour)
    {
    case Colour::Brown:
    case Colour::Blue:
        if (m_setCollection[setColour] == 2)
        {
            return true;
        }
        else
        {
            return false;
        }
    case Colour::LightBlue:
    case Colour::Magenta:
    case Colour::Orange:
    case Colour::Red:
    case Colour::Yellow:
    case Colour::Green:
        if (m_setCollection[setColour] == 3)
        {
            return true;
        }
        else
        {
            return false;
        }
    default:
        return false;
    }
}



void Player::reviewProperties()
{
    // TODO: properties should ideally cycle in board order
    // maybe go from 0 <-> 39, check owner of space
    // but player would need to know full board list
    // or all players would need an array of boardspaces which starts full of nullptr but is set to property matching index
    int index = 0; // a local index to cycle through properties
    int menuOptions = 0; // how many options are returned by property view menu (1 = un/mortgage, 2 = build/demo, 3 = all three)
    bool build = false;
    bool demo = false;
    bool mortgage = false;

    int housesAvailable = m_admin->getHousePool();
    int hotelsAvailable = m_admin->getHotelPool();

    std::shared_ptr<BoardSpace> viewedProperty;
    char menuSelection = 'q'; // just need a default value for do-while to be okay
    int housesHere = 0;

    do
    {
        system("cls");

        cout << "Reviewing " << m_playerName << "'s Properties" << endl;
        cout << "Money: $ " << m_totalMoney << endl;
        cout << "==========================" << endl;

        viewedProperty = m_ownedProperties.at(index);
        viewedProperty->printName();
        viewedProperty->displayDetails();

        housesHere = viewedProperty->houseCount();
        // print property-specific menu
        viewedProperty->viewMenu(housesAvailable, hotelsAvailable);

        // print review menu
        cout << "p - previous" << endl;
        cout << "n - next" << endl;
        cout << "q - back" << endl;
        cin >> menuSelection;

        if (menuSelection == 'p') // decrement index to view, loop to top under zero
        {
            if (index == 0)
            {
                index = m_propertyCounter - 1;
            }
            else
            {
                index--;
            }
        }
        else if (menuSelection == 'n') // increment index to view, loop to 0 if over max (size is total elements, not indices)
        {
            if (index == m_propertyCounter - 1)
            {
                index = 0;
            }
            else
            {
                index++;
            }
        }

        else if (menuSelection == 'm' && viewedProperty->canBeMortgaged()) // chose un/mortgage and can un/mortgage
        {
            bool isMortgagedNow = viewedProperty->switchMortgageState();
            if (isMortgagedNow) // true means it was not mortgaged, but is now
            {
                // true = mortgaged NOW, get money
                receiveMoney(viewedProperty->getMortgageAmount());
            }
            else
            {
                // false = not mortgaged NOW, pay money
                payMoney(viewedProperty->getUnMortgageAmount());
            }

            // rails and utilities need to be adjusted if mortgaged
            if (dynamic_pointer_cast<Railroad>(viewedProperty))
            {
                mortgagedRail(isMortgagedNow); // if this passes in true / false, all rails will be set to x - 1 / x + 1
            }
            if (dynamic_pointer_cast<Utility>(viewedProperty))
            {
                mortgagedUtility(isMortgagedNow); // if this passes in true / false, all utilities will be set to x - 1 / x + 1
            }
        }
        // chose build, build is possible with: houses left to build, full set of one colour
        else if (menuSelection == 'b' && hasCompleteSet(viewedProperty->getSetColour()) && viewedProperty->canBuildHere())
        {
            // check if resources are available for building a house or hotel
            if ((housesHere < 4 && housesAvailable > 0) || (housesHere == 4 && hotelsAvailable > 0))
            {
                if (m_totalMoney >= viewedProperty->checkBuidCost())
                {
                    if (viewedProperty->build())
                    {
                        housesHere++;
                        if (housesHere == 5) // built a hotel
                            hotelsAvailable--; // set this so admin can be updated
                        if (housesHere < 5) // built a house
                            housesAvailable--; // set this so admin can be updated

                        payMoney(viewedProperty->checkBuidCost()); // this pays the correct amount based on house number

                        cout << m_playerName << " built a " << (housesHere < 5 ? "house" : "hotel") << endl;
                    }
                    else
                    {
                        cout << "Couldn't build" << endl;
                    }
                }
                else
                {
                    cout << "Can't afford to build here" << endl;
                }
            }
        }
        // chose demo, has houses to demo, don't need to check colour set (that would be impossible to be false)
        else if (menuSelection == 'd' && housesHere > 0)
        {
            receiveMoney(viewedProperty->demolish());
            housesHere--;

            if (housesHere == 4) // demoed a hotel
                hotelsAvailable++; // set this so admin can be updated
            if (housesHere < 4) // demoed a house
                housesAvailable++; // set this so admin can be updated

            cout << m_playerName << " demolished a " << (housesHere < 4 ? "house" : "hotel") << endl;
        }
        else if (menuSelection == 'q')
        {
            break;
        }
        else
        {
            cout << "Invalid entry" << endl;
        }
        Sleep(1000);
    } while (menuSelection != 'q');

    // in case player built/demolished, update admin
    m_admin->setHousePool(housesAvailable);
    m_admin->setHotelPool(hotelsAvailable);
}

void Player::acquireProperty(std::shared_ptr<BoardSpace> aNewSpace)
{

    // add to list of properties, in the right position
    organizeNewProperty(aNewSpace);
    m_propertyCounter++;
    aNewSpace->setOwner(m_playerName);

    //m_propertiesInOrder[aNewSpace->getBoardLocation()] = aNewSpace;

    // check if specificallly a railroad
    if (dynamic_pointer_cast<Railroad>(aNewSpace))
    {
        // increment number of rails owned
        m_ownedRails++;
        // add to list of owned railroads
        m_railList.push_back(aNewSpace);

        for (int i = 0; i < m_ownedRails; i++)
        {
            // tell each owned railroad how many rails you own so rent is adjusted
            m_railList.at(i)->setRails(m_ownedRails);
        }
    }
    else if (dynamic_pointer_cast<Utility>(aNewSpace))
    {
        // increment owned utilities
        m_ownedUtilities++;
        // add to list of owned utilities
        m_utilsList.push_back(aNewSpace);

        for (int i = 0; i < m_ownedUtilities; i++)
        {
            // tell each owned railroad how many rails you own so rent is adjusted
            m_utilsList.at(i)->setRails(m_ownedUtilities);
        }
    }
    else
    {
        // normal property, add to colour collection
        m_setCollection[aNewSpace->getSetColour()]++;
        // check if amount owned is the maximum available of that colour (has full set now)
        if (m_setCollection[aNewSpace->getSetColour()] == aNewSpace->getSetSize())
        {
            updateSetSpaces(aNewSpace, true);
        }
    }
}

void Player::updateSetSpaces(std::shared_ptr<BoardSpace>& aNewSpace, bool isComplete)
{
    // tell all members of set that they are complete
    aNewSpace->updateSetStatus(isComplete);
    for (int p = 0; p < aNewSpace->getSetSize(); p++)
    {
        // get each neighbour, and tell admin to tell those properties to be complete or not
        // the alternative is to iterate through all owned properties to find the ones matching the neighbours
        m_admin->updateSetStatus(aNewSpace->getNeighbourByIndex(p), isComplete);
    }
}

void Player::organizeNewProperty(std::shared_ptr <BoardSpace> aNewSpace)
{
    int indexToInsert = -1;
    for (int i = 0; i < m_propertyCounter; i++)
    {
        // iterate through from highest board location to lowest
        // when the new one board location is higher than i, save i and break out
        if (aNewSpace->getBoardLocation() > m_ownedProperties.at(i)->getBoardLocation())
        {
            indexToInsert = i;
            break;
        }
    }

    if (indexToInsert == -1) // if insert is still -1, new number is bigger than the stored ones, so add to end
    {
        m_ownedProperties.push_back(aNewSpace);
    }
    else // otherwise, insert at the saved i value
    {
        m_ownedProperties.insert(m_ownedProperties.begin() + indexToInsert, aNewSpace);
    }
}

int Player::getPropertyCount()
{
    return m_propertyCounter;
}

void Player::forfeitProperty(std::shared_ptr<BoardSpace> aNewSpace)
{
    // remove the property from the list of owned properties
    // don't need to worry about setting the owner to none:
    // this function only happens after trading, the new player will Acquire and set the name
    // NOTE: going bankrupt doesn't require forfeiting properties,
    // a bankrupt player's property list doesn't affect anything
    //m_propertiesInOrder[aNewSpace->getBoardLocation()] = nullptr;
    for (int i = 0; i < m_propertyCounter; i++)
    {
        if (aNewSpace->getFirstName() == m_ownedProperties.at(i)->getFirstName())
        {
            m_ownedProperties.erase(m_ownedProperties.begin() + i);
        }
    }
    m_propertyCounter--;

    // if the lost property was either a Railroad or Utility, need to update any remaining
    if (dynamic_pointer_cast<Railroad>(aNewSpace))
    {
        m_ownedRails--;
        for (int i = 0; i < m_railList.size(); i++) // if a rail is removed, will this go to an index that doesn't exist? or re-evaluate?
        {
            m_railList.at(i)->setRails(m_ownedRails);

            if (m_railList.at(i)->getFirstName() == aNewSpace->getFirstName())
            {
                m_railList.erase(m_railList.begin() + i);
                break;
            }
        }
    }
    else if (dynamic_pointer_cast<Utility>(aNewSpace))
    {
        m_ownedUtilities--;
        for (int i = 0; i < m_utilsList.size(); i++)
        {
            m_utilsList.at(i)->setUtility(m_ownedUtilities);

            if (m_utilsList.at(i)->getFirstName() == aNewSpace->getFirstName())
            {
                m_utilsList.erase(m_utilsList.begin() + i);
                break;
            }
        }
    }
    else
    {
        // normal property, remove from colour collection
        m_setCollection[aNewSpace->getSetColour()]--;
        if (m_setCollection[aNewSpace->getSetColour()] > 0)
        {
            // doesn't matter how many you have, if you lost one but still have at least one, no one has a full set
            // alternatively, if you had one and traded it to someone with two, this could erroneously set the full set to false
            // even though the other player now has the whole set
            updateSetSpaces(aNewSpace, false);
        }
    }
}

int Player::checkOwnedUtilities()
{
    return m_ownedUtilities;
}

void Player::modifiyOwnedUtilities(int val)
{
    m_ownedUtilities += val; // negative for losing one
}

int Player::auctionOptions(int currentHighBid)
{
    int menuOption = 4;
    if (isInJail())
    {
        cout << "Players in Jail cannot participate" << endl;
        return -1;

    }
    // only the options the player can afford will be listed, in ascending order
    // this will count how many options are shown
    // this is used for error checking (player can't select an option that he isn't offered)
    // otherwise, even if player can't afford $100 and #3 doesn't show up, he could enter 3 anyway
    int auctionOptionCount = 0;
    cout << m_playerName << " selecting..." << endl;
    if (m_totalMoney >= currentHighBid + 1)
    {
        auctionOptionCount++;
        cout << "1 - Raise bid +$1" << endl;
    }
    if (m_totalMoney >= currentHighBid + 10)
    {
        auctionOptionCount++;
        cout << "2 - Raise bid +$10" << endl;
    }
    if (m_totalMoney >= currentHighBid + 100)
    {
        auctionOptionCount++;
        cout << "3 - Raise bid +$100" << endl;
    }
    cout << "0 - Withdraw from auction" << endl;
    do
    {
        cout << "Select one:" << endl;
        cin >> menuOption;
        if (menuOption > auctionOptionCount || menuOption < 0)
            cout << "Invalid entry." << endl;

    } while (menuOption > auctionOptionCount || menuOption < 0);

    switch (menuOption)
    {
    case 1:
        m_auctionBid = currentHighBid + 1;
        break;
    case 2:
        m_auctionBid = currentHighBid + 10;
        break;
    case 3:
        m_auctionBid = currentHighBid + 100;
        break;
    case 0:
        cout << m_playerName << " withdraws from the auction" << endl;
        m_auctionBid = -1;
        break;
    }
    return m_auctionBid;
}

int Player::getAuctionBid()
{
    return m_auctionBid;
}

void Player::resetAuction()
{
    m_auctionBid = 0;
}

bool Player::hasBonusRoll()
{
    return m_hasBonusRoll;
}

void Player::mortgagedUtility(bool mortgaged)
{// 
    if (mortgaged)
        m_ownedUtilities--;
    if (!mortgaged)
        m_ownedUtilities++;

    // regardless of whether the number went up or down, tell all owned utilities the new value
    // the mortgaged one(s) will still have this value, but mortgaged properties won't charge rent
    for (int i = 0; i < m_ownedRails; i++)
    {
        m_utilsList.at(i)->setUtility(m_ownedUtilities);
    }
}

void Player::mortgagedRail(bool mortgaged)
{
    // 
    if (mortgaged)
        m_ownedRails--;
    if (!mortgaged)
        m_ownedRails++;

    // regardless of whether the number went up or down, tell all owned rails the new value
    // the mortgaged one(s) will still have this value, but mortgaged properties won't charge rent
    for (int i = 0; i < m_ownedRails; i++)
    {
        m_railList.at(i)->setRails(m_ownedRails);
    }
}

void Player::setJailCards(int newVal)
{
    m_jailCards += newVal;
}

int Player::countJailCards()
{
    return m_jailCards;
}

tradeOffer Player::updateTrade(tradeOffer t)
{
    int menuSelection = 0;
    int moneyAmount = 0;
    int jailAmount = 0;
    do
    {

        do
        {
            cout << "1 - money" << endl;
            cout << "2 - jail cards" << endl;
            cout << "3 - properties" << endl;
            cout << "4 - return" << endl;
            cin >> menuSelection;

        } while (menuSelection < 1 && menuSelection > 4);

        switch (menuSelection)
        {
        case 1:
            cout << "Current: " << t.tradeMoney << endl;
            cout << "New Amount (available: $" << t.trader->getCurrentFunds() << ")" << endl;
            cin >> moneyAmount;
            if (moneyAmount > t.trader->getCurrentFunds())
            {
                cout << "Invalid amount" << endl;
            }
            else
            {
                t.tradeMoney = moneyAmount;
            }
            break;
        case 2:
            cout << "Current: " << t.tradeJailCards << endl;
            cout << "New Amount (available: " << t.trader->countJailCards() << ")" << endl;
            cin >> jailAmount;
            if (jailAmount > t.trader->countJailCards())
            {
                cout << "Invalid amount" << endl;
            }
            else
            {
                t.tradeJailCards = jailAmount;
            }
            break;
        case 3:
            // review properties of t.trader
            // add properties to t.properties
            // or remove properties from t.properties
            // property needs a bool to indicate it is part of trade
            // this will need to be reset after trade finalized

            int index = 0;
            char menuSelection = 'q';
            std::shared_ptr<BoardSpace> viewedProperty;
            int propertyCounter = t.trader->m_propertyCounter;

            do
            {
                system("cls");

                cout << "Reviewing " << m_playerName << "'s Properties" << endl;
                cout << "Money: $ " << m_totalMoney << endl;
                cout << "==========================" << endl;

                viewedProperty = t.trader->m_ownedProperties.at(index);
                viewedProperty->printName();
                viewedProperty->displayDetails();

                if (viewedProperty->isTradeListed())
                {
                    cout << "t - remove from trade" << endl;
                }
                else
                {
                    cout << "t - add to trade" << endl;
                }

                cout << "============" << endl;

                // print review menu
                cout << "p - previous" << endl;
                cout << "n - next" << endl;
                cout << "q - back" << endl;
                cin >> menuSelection;

                if (menuSelection == 'p') // decrement index to view, loop to top under zero
                {
                    if (index == 0)
                    {
                        index = propertyCounter - 1;
                    }
                    else
                    {
                        index--;
                    }
                }
                else if (menuSelection == 'n') // increment index to view, loop to 0 if over max (size is total elements, not indices)
                {
                    if (index == propertyCounter - 1)
                    {
                        index = 0;
                    }
                    else
                    {
                        index++;
                    }
                }
                else if (menuSelection == 't')
                {

                    viewedProperty->updateTradeList();
                    if (viewedProperty->isTradeListed())
                    {
                        // add to trade struct
                        // this is the exact same code as organizeNewProperty
                        // if i can modify that function to work with this, it will cut down on lines
                        // EDIT: organizeNewProperty can't take t.properties, unless by reference
                        // otherwise, it will only be changed locally
                        // but then that function would have to take the player's own property list, by reference,
                        // every time a new property is acquired
                        int indexToInsert = -1;
                        int propertyCount = t.tradeProperties.size();
                        for (int i = 0; i < propertyCount; i++)
                        {
                            // iterate through from highest board location to lowest
                            // when the new one board location is higher than i, save i and break out
                            if (viewedProperty->getBoardLocation() > m_ownedProperties.at(i)->getBoardLocation())
                            {
                                indexToInsert = i;
                                break;
                            }
                        }

                        if (indexToInsert == -1) // if insert is still -1, new number is bigger than the stored ones, so add to end
                        {
                            t.tradeProperties.push_back(viewedProperty);
                        }
                        else // otherwise, insert at the saved i value
                        {
                            t.tradeProperties.insert(m_ownedProperties.begin() + indexToInsert, viewedProperty);
                        }

                    }
                    else
                    {
                        // remove from trade struct, have to search for match
                        for (int i = 0; i < t.tradeProperties.size(); i++)
                        {
                            if (t.tradeProperties.at(i) == viewedProperty)
                            {
                                t.tradeProperties.erase(t.tradeProperties.begin() + i);
                                break;
                            }
                        }
                    }
                }
                else if (menuSelection == 'q')
                {
                    break;
                }

            } while (menuSelection != 'q');
            break;
        }
    } while (menuSelection != 4);



    return t;
}

void Player::performTrade(tradeOffer out, tradeOffer in)
{
    // out = the finalized struct going to the other player, remove from owner
    payMoney(out.tradeMoney);
    setJailCards(out.tradeJailCards * -1); // this function is set to receive +/- val so set to negative


    for (int t = 0; t < out.tradeProperties.size(); t++) // iterate through the properties to lose
    {
        for (int p = 0; p < m_propertyCounter; p++) // iterate through owned properties to compare, this list will become shorter
        {
            if (out.tradeProperties[t] == m_ownedProperties[p])
            {
                forfeitProperty(m_ownedProperties[p]); // this could be either t or p, they're literally the same thing
                // m_propertyCounter will be decremented in the forfeit function
                break; // move on to the next t
            }
        }
    }

    // in  = the finalized struct coming from the other player, add to owner
    receiveMoney(in.tradeMoney); // adds to money
    setJailCards(in.tradeJailCards); // adds in value to total
    for (int i = 0; i < in.tradeProperties.size(); i++)
    {
        acquireProperty(in.tradeProperties.at(i)); // sets name as owner
    }

}

char Player::playerMenu(bool includePass, bool includeTrade, std::shared_ptr<Player> pOwner)
{
    char menuSelection = 'r';

    if (getPropertyCount() > 0)
        cout << "r - review properties" << endl;

    if (includeTrade)
        cout << "t - trade properties" << endl;

    if (includePass)
        cout << "p - pass turn" << endl;

    cout << "Q - declare bankruptcy" << endl;

    cin >> menuSelection;

    if (menuSelection == 'r')
    {
        if (getPropertyCount() > 0)
        {
            reviewProperties();

        }
    }
    else if (menuSelection == 't' && includeTrade)
    {
        int chosenPartner = -1;
        // prompt user to choose a player to trade with
        do
        {
            cout << "Who To Trade With:" << endl;
            for (int i = 0; i < m_admin->getPlayerCount(); i++)
            {
                if (i != m_admin->getCurrentPlayerIndex())
                {
                    // only print names and user-friendly indexes of other players
                    //printf("%i - %s", i + 1, admin->getPlayerList().at(i));
                    cout << i + 1 << " " << m_admin->getPlayerList().at(i)->m_playerName << endl;
                }
            }
            cin >> chosenPartner;
            chosenPartner--;

            // loop until they don't choose themself
        } while (chosenPartner == m_admin->getCurrentPlayerIndex());
        // there's literally no benefit to asking GameAdmin for this reference to pass into GameAdmin
        //std::shared_ptr<Player> tradePartner = admin->getPlayerList().at(chosenPartner);
        m_admin->handleTrade(chosenPartner);
    }

    else if (menuSelection == 'Q')
    {
        cout << "Q - confirm bankruptcy" << endl;
        cin >> menuSelection;
        if (menuSelection == 'Q')
        {
            if (m_inDebtToPlayer)
            {
                goBankrupt(m_inDebtToPlayer);
            }
            else
            {
                goBankrupt();
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