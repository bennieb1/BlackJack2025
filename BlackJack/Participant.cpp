#include "Participant.h"
#include "Strategy.h"

Participant::Participant(const std::string& n, Strategy* b)
    : label_(n), brain_(b) {}
Participant::~Participant(){ delete brain_; }

void Participant::clearHand(){ state_.hand.clear(); }

bool Participant::placeBet(){
    int w = brain_->bet(purse_.credits);
    if(w<=0||!purse_.withdraw(w)){ state_.stake=0; return false; }
    state_.stake = w; return true;
}

bool Participant::wantsHit(const State& dealer,const TableRules& r){
    return brain_->hit(state_,dealer,r);
}
bool Participant::wantsSplit(const TableRules& r){
    return brain_->split(state_,r);
}
bool Participant::wantsDbl(const TableRules& r){
    return brain_->dbl(state_,r);
}
bool Participant::canSplit(const TableRules&) const{
    return state_.hand.size()==2 &&
           state_.hand[0].rank==state_.hand[1].rank;
}
