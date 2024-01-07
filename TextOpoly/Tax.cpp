#include "Tax.h"
#include <iostream>

using namespace std;

void Tax::displayDetails()
{
    cout << formatDisplayNameRow("Pay $" + to_string(m_taxAmount), 18) << endl;
}

int Tax::getRentFee()
{
    return m_taxAmount;
}