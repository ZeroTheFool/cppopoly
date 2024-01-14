#pragma once

enum class functionType
{
    Receive,
    Pay,
    MoveToIndex,
    MoveAndPay,
    GoToJail,
    GainJailCard
};

enum class functionTarget
{
    None,
    All,
    Bank
};

enum class PropertyResult
{
    None,
    BoughtNew,
    PaidRent,
    AuctionNeeded,
    Bankruptcy,
    SentToJail
};

enum class Colour
{
    None,
    Brown,
    LightBlue,
    Magenta,
    Orange,
    Red,
    Yellow,
    Green,
    Blue
};