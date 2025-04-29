#include "Participant.h" 
#include "Strategy.h"
#include "Card.h"
#include <cctype>
#include <iostream>
#include <limits>
#include <string>


int ConsoleAI::bet(int bank){
    if(bank<=0) return 0;
    int b; std::cout<<"Bank"<<bank<<". bet: "; std::cin>>b;
    return (b>0 && b<=bank)? b : 0;
}

bool ConsoleAI::hit(const State& self,const State& dealer,const TableRules&){
    showHand("You",&self.hand,false);
    showHand("Dealer",&dealer.hand,true);
    std::cout<<"Hit?  (y or h = yes, anything else = stand): ";

    std::string line;
    std::getline(std::cin >> std::ws, line);       // flush \n then read line
    char c = line.empty() ? 'N'
                          : std::toupper(static_cast<unsigned char>(line[0]));
    return c=='Y'||c=='H';
}

bool ConsoleAI::split(const State& s,const TableRules&){
    if(!(s.hand.size()==2 && s.hand[0].rank==s.hand[1].rank)) return false;
    std::cout<<"Split pair? (y/n): ";
    char c; std::cin>>std::ws>>c;
    return std::toupper(static_cast<unsigned char>(c))=='Y';
}

bool ConsoleAI::dbl(const State& s,const TableRules&){
    if (s.hand.size() != 2) return false;         

    std::cout << "Double-down (double bet, one card)? (y/n): ";
    char c; std::cin >> std::ws >> c;
    return std::toupper(static_cast<unsigned char>(c)) == 'Y';
}


bool DealerAI::hit(const State& self,const State&,const TableRules& r){
    return handValue(&self.hand) < r.dealerStand;
}
