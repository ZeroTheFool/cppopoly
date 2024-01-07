/*
//////chance cards
N/A get out of jail free

PAY pay poor tax $15
PAYALL elected chairman of the board, pay each player $50
PAY assessed for repairs - $50 / house $100 / hotel
REC bank pays div $50
REC building and loan matures, collect $150

GTJ go to jail - don't pass go

MOV go back three spaces

MOV advance to boardwalk
MOV advance to go $200
MOV go to reading railroad, $200 if pass go
MOV advance to st charles, $200 if pass go
MOV advance to illinois, $200 if pass go

MPE advance to nearest utility - buy or pay owner 10 times new die roll

MAP advance to nearest railroad - buy or pay double rent
MAP advance to nearest railroad - buy or pay double rent


//////community chest
REC inherit $100
REC receive $25 consultancy fee
REC life insurance matures, collect $100
REC won 2nd price beauty contest, gain $10
REC income tax refund $20
REC bank error, receive $200
REC sale of stock, gain $50
RECALL opera night : gain $50 from each player for opening night seats
REC holiday christmas fund matures, gain $100

MOV advnace to go, $200

PAY pay school fees tax $150
PAY doctor's fees, pay $50
PAY assessed for repairs - $50 / house $100 / hotel
PAY pay hospital fees $100

GTJ go to jail, don't pass go

N/A get out of jail free



Constructor:
cardName, type(MOV,PAY,REC,GTJ,GJF,MAP) determines function, cardText, locationIndex if applicable, $cost if applicable

i think there are really only seven card types
-PAY description, payMoney
-REC description, recMoney
-MOV description, go to locationIndex
-GTF description, player->gotojail(true)
-GJF description, player->jailcards(+1)
-MAI description, go to locationIndex, handleProperty
-MAP description, go to locationIndex, pay multiplier
-pay for repairs might need it's own function
*/