#include "Corner.h"
#include <iostream>

using namespace std;

void Corner::displayDetails()
{
    cout << m_description << endl;
}

bool Corner::checkIsSendJail()
{
    return m_isSendJail;
}
