#pragma once
struct Payout {            
    int normal  = 2;       
    int natural = 3;      
    int push    = 1;      
};
struct TableRules {
    int blackjack   = 21;
    int dealerStand = 17;
    Payout pay;
};
