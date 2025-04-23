#pragma once
#include "Participant.h"
#include "Card.h"

class Dealer: public Participant
{
    
public:

    std::string name() const override{return "Dealer";}

    bool wantsCards(const Participant& ) override;
};
