#pragma once
#include "Card.h"
#include "Wallet.h"
#include <string>
#include <vector>

#include "Rules.h"

struct State { Hand hand; int stake = 0; };
class Strategy;

class Participant {
public:
    Participant(const std::string& name, Strategy* brain);
    ~Participant();

    bool placeBet();
    void clearHand();
    void giveCards(const Card& c){ state_.hand.push_back(c); }

    bool wantsHit   (const State& dealer,const TableRules&);
    bool wantsSplit (const TableRules&);
    bool wantsDbl   (const TableRules&);
    bool canSplit   (const TableRules&) const;

    const std::string& name() const { return label_; }
    Wallet&            wallet()     { return purse_; }
    State&             state()      { return state_; }

    std::vector<State*> splitHands;          // extra hands after split

private:
    std::string label_;
    Wallet      purse_;
    State       state_;
    Strategy*   brain_;
};
