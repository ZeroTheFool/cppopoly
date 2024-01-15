#include "CardSystem.h"
#include "GameCard.h"

#include <array>

void CardSystem::makeChanceDeck()
{
    std::array<std::string, 6> m_descriptionLines = {"","","","","",""};
    //////chance cards
    //    N/A 
    //    get out of jail free <- 20 characters - new max size
    m_descriptionLines[0] = "Get Out of Jail";
    m_descriptionLines[1] = "Free";
    m_descriptionLines[1] = "";
    chanceDeck[0] = new GameCard("Chance Card", m_descriptionLines, 0, 1, functionType::GainJailCard, functionTarget::None);
    //    PAY
    //    pay poor tax $15 <- 16 characters
    m_descriptionLines[0] = "Pay Poor Tax: $15";
    m_descriptionLines[1] = "";
    chanceDeck[1] = new GameCard("Chance Card", m_descriptionLines, 15, 1, functionType::Pay, functionTarget::Bank);
    // 
    //    assessed for repairs
    //    $50 / house
    //    $100 / hotel
    m_descriptionLines[0] = "Assessed For Repairs";
    m_descriptionLines[1] = "Pay $50 per House";
    m_descriptionLines[2] = "Pay $100 per Hotel";
    m_descriptionLines[3] = "";
    chanceDeck[2] = new GameCard("Chance Card", m_descriptionLines, 15, -1, functionType::Pay, functionTarget::Bank);
    //    PAYALL 
    //    elected chairman
    //    of the board:
    //    pay each player $50 <- 19 characters
    m_descriptionLines[0] = "Elected Chairman";
    m_descriptionLines[1] = "of the Board:";
    m_descriptionLines[2] = "Pay Each Player $50";
    m_descriptionLines[3] = "";
    chanceDeck[3] = new GameCard("Chance Card", m_descriptionLines, 50, 1, functionType::Pay, functionTarget::All);
    
    //    REC
    //    bank pays out
    //    dividend of $50
    m_descriptionLines[0] = "Bank Pays Out";
    m_descriptionLines[1] = "Dividend of $50";
    m_descriptionLines[2] = "";
    chanceDeck[4] = new GameCard("Chance Card", m_descriptionLines, 50, 1, functionType::Receive, functionTarget::Bank);

    // 
    //    loan matures,
    //    collect $150
    m_descriptionLines[0] = "Loan Matures";
    m_descriptionLines[1] = "Collect $150";
    m_descriptionLines[2] = "";
    chanceDeck[5] = new GameCard("Chance Card", m_descriptionLines, 150, 1, functionType::Receive, functionTarget::Bank);

    //    GTJ
    //    go directly to jail <- 19 characters
    //    do not pass go
    m_descriptionLines[0] = "Go Directly to Jail";
    m_descriptionLines[1] = "Do Not Pass Go";
    m_descriptionLines[2] = "Do Not Collect $200";
    m_descriptionLines[3] = "";
    chanceDeck[6] = new GameCard("Chance Card", m_descriptionLines, 0, 1, functionType::GoToJail, functionTarget::None);

    //    MOV
    //    move back
    //    three spaces
    m_descriptionLines[0] = "Move Back";
    m_descriptionLines[1] = "Three Spaces";
    m_descriptionLines[2] = "";
    chanceDeck[7] = new GameCard("Chance Card", m_descriptionLines, -3, 1, functionType::MoveToIndex, functionTarget::None);

    //    MOV
    //    advance to boardwalk
    m_descriptionLines[0] = "Advance to";
    m_descriptionLines[1] = "Boardwalk";
    m_descriptionLines[2] = "";
    chanceDeck[8] = new GameCard("Chance Card", m_descriptionLines, 39, 1, functionType::MoveToIndex, functionTarget::None);

    //    advance to go $200
    m_descriptionLines[0] = "Advance to";
    m_descriptionLines[1] = "Go";
    m_descriptionLines[2] = "Collect $200";
    m_descriptionLines[3] = "";
    chanceDeck[9] = new GameCard("Chance Card", m_descriptionLines, 0, 1, functionType::MoveToIndex, functionTarget::None);
    //    
    //    
    //          go to
    //    reading railroad,
    //        gain $200
    //     if you pass go
    m_descriptionLines[0] = "Advance to";
    m_descriptionLines[1] = "Reading Railroad";
    m_descriptionLines[2] = "gain $200";
    m_descriptionLines[3] = "if you pass Go";
    m_descriptionLines[4] = "";
    chanceDeck[10] = new GameCard("Chance Card", m_descriptionLines, 5, 1, functionType::MoveToIndex, functionTarget::None);
    //    
    //       advance to
    //    st charles place
    //       gain $200
    //     if you pass go
    m_descriptionLines[0] = "Advance to";
    m_descriptionLines[1] = "St Charles Place";
    m_descriptionLines[2] = "gain $200";
    m_descriptionLines[3] = "if you pass Go";
    m_descriptionLines[4] = "";
    chanceDeck[11] = new GameCard("Chance Card", m_descriptionLines, 11, 1, functionType::MoveToIndex, functionTarget::None);

    //      advance to
    //     illinois ave
    //      gain $200
    //    if you pass go
    m_descriptionLines[0] = "Advance to";
    m_descriptionLines[1] = "Illinois Avenue";
    m_descriptionLines[2] = "gain $200";
    m_descriptionLines[3] = "if you pass Go";
    m_descriptionLines[4] = "";
    chanceDeck[12] = new GameCard("Chance Card", m_descriptionLines, 24, 1, functionType::MoveToIndex, functionTarget::None);

    //    MPE
    //       advance to
    //    nearest utility
    //     buy if unowned
    //          or
    //       pay owner
    //    10x new die roll
    m_descriptionLines[0] = "Advance to";
    m_descriptionLines[1] = "nearest Utility";
    m_descriptionLines[2] = "Buy if Unowned";
    m_descriptionLines[3] = "or";
    m_descriptionLines[4] = "Pay Owner";
    m_descriptionLines[5] = "10x New Die Roll";
    chanceDeck[13] = new GameCard("Chance Card", m_descriptionLines, -1, 10, functionType::MoveAndPay, functionTarget::Single);

    //    MAP
    //       advance to
    //    nearest railroad
    //     buy if unowned 
    //          or 
    //    pay double rent
    m_descriptionLines[0] = "Advance to";
    m_descriptionLines[1] = "nearest Railroad";
    m_descriptionLines[2] = "Buy if Unowned";
    m_descriptionLines[3] = "or";
    m_descriptionLines[4] = "Pay Double Rent";
    m_descriptionLines[5] = "";
    chanceDeck[14] = new GameCard("Chance Card", m_descriptionLines, -1, 2, functionType::MoveAndPay, functionTarget::Single);
    //       advance to
    //    nearest railroad
    //     buy if unowned 
    //          or 
    //    pay double rent
    chanceDeck[15] = new GameCard("Chance Card", m_descriptionLines, -1, 2, functionType::MoveAndPay, functionTarget::Single);

}

void CardSystem::makeCommunityDeck()
{
    //communityDeck[0] = new GameCard("get 50", "you get 50", 50, 1, functionType::Receive, functionTarget::Bank);
    //communityDeck[1] = new GameCard("pay 50", "you pay 50", 50, 1, functionType::Pay, functionTarget::All);
    //communityDeck[2] = new GameCard("move to Go", "go to Go", 0, 1, functionType::MoveToIndex, functionTarget::None);
}