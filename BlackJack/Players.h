#pragma once
#include <iostream>
#include <memory>

#include "Participant.h"


class Players : public Participant
{
public:
    explicit Players(std::string label)
       : label(std::move(label)), credit(1000), bet(0) {}

   
    bool placeBet();              
    void settle(int delta);       
    int  wager()  const { return bet; }
    int  credits() const { return credit; }

   
    bool wantsCards (const Participant& dealer) override;
    bool wantsSplit(const Hands& hand) const;
    static bool canSplit(const Hands& hand)
    {
        return hand.size() == 2 &&
               hand[0].rank == hand[1].rank;
    }
    bool wantsDblDn() const;

    std::string name() const override { return label; }
 

private:
    std::string label;
    int credit;   
    int bet;    
};
