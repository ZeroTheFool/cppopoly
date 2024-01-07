#include "Utility.h"
#include <iostream>

using namespace std;

void Utility::displayDetails()
{
    if (m_isMortgaged)
        cout << formatDisplayNameRow("--MORTGAGED--", 18) << endl;

    if (m_currentOwner == "None")
    {
        cout << formatDisplayNameRow("Price:" + to_string(m_purchasePrice), 18) << endl;
        cout << formatDisplayNameRow("Rent: Roll x " + to_string(rentMultipier[utilitiesOwned]), 18) << endl;
    }
    else
    {
        cout << formatDisplayNameRow("Owner:" + BoardSpace::m_currentOwner, 18) << endl;
        if (!m_isMortgaged)
            cout << formatDisplayNameRow("Rent: Roll x " + to_string(rentMultipier[utilitiesOwned]), 18) << endl;
    }

    cout << "\nx----------------x\n" << endl;
}

int Utility::viewMenu()
{
    // mortgaged utilities don't count toward the total owned utilties
    // if this state changes elsewhere, the player updates their owned utilities
    if (!m_isMortgaged)
    {
        cout << "m - mortgage" << "(+$" << m_mortgageAmount << ")" << endl;
    }
    else
    {
        cout << "m - unmortgage" << "(-$" << m_unMortgageAmount << ")" << endl;
    }
    cout << "======================" << endl;

    return 1;
}


void Utility::setUtility(int newVal)
{
    utilitiesOwned = newVal;
}

int Utility::getRentFee(int rollSum)
{
    // returns the value rolled, x4 or x10
    return rollSum * rentMultipier[utilitiesOwned];
}
