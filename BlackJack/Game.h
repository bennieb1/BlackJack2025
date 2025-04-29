#pragma once
#include "Card.h"
#include "Rules.h"
#include "Participant.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();
    void play();
private:
    enum class Phase { Betting, Deal, Players, Dealer, Settle };

    void next();
    void betting();
    void initialDeal();
    void playersTurn();
    void dealerTurn();
    void settle();
    void pay(State& seat, Wallet& purse, int dealerTot);

    std::vector<Participant*> seats;
    std::vector<Participant*> active;
    Participant* dealer;
    Deck* shoe;
    Phase phase;
    TableRules rules;
};
