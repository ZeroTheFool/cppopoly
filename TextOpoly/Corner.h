#pragma once
#include "BoardSpace.h"
class Corner :
    public BoardSpace
{
    /*
    this class will be one of the following:
    -Jail (has no effect on landing on it, or starting your turn on it, being in jail is controlled elsewhere)
    -Free Parking (has no effect, house rules be damned)
    -Go To Jail (tell the player to go to jail, they handle that themselves)
    -GO (has no effect, passing GO has to be handled by the roll function)

    
    so all this class needs to do is exist, and be Go TO Jail or not
    */
private:
    bool m_isSendJail = false;
    std::string m_description;

public:

    Corner() {};
    Corner(std::string inStreetName, std::string inStreetType, bool inSendJail, std::string inDescription, int inBoardLocation = 0)
        : BoardSpace(inStreetName, inStreetType, 0, "BANK", 0, 0, inBoardLocation)
    {
        // this one line determines if nothing happens or player goes to jail
        m_isSendJail = inSendJail;
        m_description = inDescription;
    };

    void displayDetails();

    bool checkIsSendJail();

};

