#include "Game.h"
#include "Strategy.h"
#include <iostream>
#include <limits>


static bool askYesNo(const char* q){
    char c=0; std::cout<<q<<" (y/n): "; std::cin>>std::ws>>c;
    return c=='y'||c=='Y';
}


static State* makeSplit(State* first, Deck*& shoe)
{
    auto* second = new State;
    second->stake = first->stake;

    second->hand.push_back(first->hand.back());
    first ->hand.pop_back();

    first ->hand.push_back(shoe->back()); shoe->pop_back();
    second->hand.push_back(shoe->back()); shoe->pop_back();
    return second;
}


Game::Game():dealer(nullptr),shoe(nullptr),phase(Phase::Betting){
    int n=0;
    while(n<1||n>7){
        std::cout<<"Enter number of players (1-7): ";
        if(std::cin>>std::ws>>n && n>=1&&n<=7) break;
        std::cout<<"Invalid.\n"; std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        n=0;
    }
    for(int i=1;i<=n;++i)
        seats.push_back(new Participant("Player "+std::to_string(i),new ConsoleAI));
    dealer=new Participant("Dealer",new DealerAI);
    seats.push_back(dealer);
}
Game::~Game(){ delete shoe; for(auto*p:seats) delete p; }


void Game::play(){ for(;;){ if(!shoe) shoe=makeShuffledDeck(); next(); }}


void Game::next(){
    switch(phase){
        case Phase::Betting : betting();      break;
        case Phase::Deal    : initialDeal();  break;
        case Phase::Players : playersTurn();  break;
        case Phase::Dealer  : dealerTurn();   break;
        case Phase::Settle  : settle();       break;
    }
}


void Game::betting(){
    std::cout<<"\n ====== New Round ======\n";
    active.clear();
    for(auto*p:seats) if(p!=dealer && p->placeBet()) active.push_back(p);
    if(active.empty()){ std::cout<<"No bets.\n"; std::exit(0);}
    phase = Phase::Deal;
}

void Game::initialDeal(){
    for(auto*p:seats) p->clearHand();
    for(int r=0;r<2;++r) for(auto*p:seats){
        p->giveCards(shoe->back()); shoe->pop_back();
    }
    phase = Phase::Players;
}

void Game::playersTurn()
{
    for (auto* p : active)
    {
        std::vector<State*> todo{ &p->state() };

        while (!todo.empty())
        {
            State* h = todo.back();
            todo.pop_back();

            bool done = false;
            while (!done)
            {
                showHand(p->name(),      &h->hand, false);
                showHand(dealer->name(), &dealer->state().hand, true);

                
                if (h->hand.size() == 2 &&
                    p->canSplit(rules)   &&
                    p->wantsSplit(rules))
                {
                    State* s = makeSplit(h, shoe);
                    p->splitHands.push_back(s);
                    todo.push_back(s);
                    continue;                      
                }

              
                if (h->hand.size() == 2 &&
                    p->wantsDbl(rules)    &&
                    p->wallet().withdraw(h->stake))
                {
                    h->stake *= 2;
                    h->hand.push_back(shoe->back()); shoe->pop_back();
                    showHand(p->name(), &h->hand, false);
                    break;                            // auto-stand
                }

                if (!p->wantsHit(dealer->state(), rules)) break;

              
                h->hand.push_back(shoe->back()); shoe->pop_back();
                showHand(p->name(), &h->hand, false);

                if (handValue(&h->hand) >= rules.blackjack) done = true;
            }
        }
    }
    phase = Phase::Dealer;
}

void Game::dealerTurn(){
    while(dealer->wantsHit(dealer->state(),rules)){
        dealer->giveCards(shoe->back()); shoe->pop_back();
    }
    phase = Phase::Settle;
}

void Game::settle()
{
    int dTot = handValue(&dealer->state().hand);
    std::cout<<"\nDealer "; showHand("Final",&dealer->state().hand,false);

    for(auto* p : active)
    {
        
        std::cout<<p->name();
        pay(p->state(), p->wallet(), dTot);
        p->state().stake = 0;

        
        int idx = 1;
        for(State* s : p->splitHands){
            std::cout<<p->name()<<" (split "<<idx++<<")";
            pay(*s, p->wallet(), dTot);
            delete s;
        }
        p->splitHands.clear();
    }

    delete shoe; shoe=nullptr;
    phase = askYesNo("\nPlay again") ? Phase::Betting
                                     : (std::exit(0), Phase::Betting);
}


void Game::pay(State& seat,Wallet& purse,int dealerTot){
    int v = handValue(&seat.hand), b = seat.stake, cb = 0;
    bool nat = (v==rules.blackjack && seat.hand.size()==2);
    if(v>rules.blackjack) cb=0;
    else if(nat && dealerTot!=rules.blackjack) cb=b*rules.pay.natural;
    else if(dealerTot>rules.blackjack || v>dealerTot) cb=b*rules.pay.normal;
    else if(v==dealerTot) cb=b*rules.pay.push;
    purse.deposit(cb);
    std::cout<<" ==== "<<cb-b<<" (Bankroll "<<purse.credits<<")\n";
}
