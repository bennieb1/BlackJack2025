#pragma once
#include<string>
#include "Card.h"

class Participant
{
public:
    virtual ~Participant() = default;

    virtual std::string name() const = 0;

    virtual bool wantsCards(const Participant& dealer) = 0;

    void takeCards(Deck& deck)
    {
        hand.push_back(deck.back());
        deck.pop_back();
    }

    const Hands* handPtr() const{return &hand;}

    Hands* handPtr() { return &hand; }


protected:

    Hands hand;
    
};
