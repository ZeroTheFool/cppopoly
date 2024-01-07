#include "Railroad.h"
#include <iostream>

using namespace std;

void Railroad::displayDetails()
{
    if (m_isMortgaged)
        cout << formatDisplayNameRow("--MORTGAGED--", 18) << endl;

    if (m_currentOwner == "None")
    {
        cout << formatDisplayNameRow("Price:" + to_string(m_purchasePrice), 18) << endl;
        cout << formatDisplayNameRow("Rent:" + to_string(m_rentFee[m_rails]), 18) << endl;
    }
    else
    {
        cout << formatDisplayNameRow("Owner:" + BoardSpace::m_currentOwner, 18) << endl;
        if (!m_isMortgaged)
            cout << formatDisplayNameRow("Rent:" + to_string(m_rentFee[m_rails]), 18) << endl;
    }

    cout << "\nx----------------x\n" << endl;
}

int Railroad::viewMenu()
{
    // mortgaged rails shouldn't count toward total owned for rent
    // when this state changes elsewhere, the player updates their owned rails
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

int Railroad::getRentFee()
{
    // return fee index using number of rails
    return m_rentFee[m_rails];
}

void Railroad::setRails(int newVal)
{
    m_rails = newVal;
}

