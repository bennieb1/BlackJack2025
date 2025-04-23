
#include "Players.h"
#include "Card.h"
#include "Dealer.h"

bool askYesNo(const char* prompt)
{
    char c = 0;
    std::cout << prompt << " (y/n): ";
    std::cin  >> c;
    return c == 'y' || c == 'Y';
}

int main(int argc, char* argv[])
{

    int seats = 0;

    while (seats < 1 || seats > 7) {
        std::cout << "Enter number of players (1-7): ";
        std::cin >> seats;

       
        if (!std::cin || seats < 1 || seats > 7) {
            std::cout << "Please enter a number from 1-7.\n";
            std::cin.clear();                                    
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            seats = 0;                                            
        }
    }



    std::vector<Players*> players;
    std::vector<Participant*> participants;

    Dealer dealer;
    Participant* house = &dealer;

    for (int i = 0; i < seats; ++i)
    {
        auto* p = new Players("Player " + std::to_string(i + 1));
        players.push_back(p);
        participants.push_back(p);
    }

    bool playMore = true;
    while (playMore)
    {
        Deck* deck = makeShuffledDeck();

        participants.erase(std::remove_if(participants.begin(), participants.end(),
            [](Participant* pt){ return false; }), participants.end()); // keep size

        std::vector<Players*> active;   // those who actually bet
        for (auto* p : players)
            if (p->placeBet()) active.push_back(p);

        if (active.empty()) {           // no one bet ⇒ quit round
            delete deck;
            if (!(playMore = askYesNo("Nobody bet; play again"))) break;
            else continue;
        }

     
        for (auto* p : players) p->handPtr()->clear();
        house->handPtr()->clear();
        for (int r = 0; r < 2; ++r) {
            for (auto* p : active) p->takeCards(*deck);
            house->takeCards(*deck);
        }

        
        for (auto* p : active) {
            std::cout << "\n       " << p->name() << "         \n";
            while (handValue(p->handPtr()) < 21 && p->wantsCards(*house))
                p->takeCards(*deck);
        }

       
        std::cout << "\n   Dealer reveals  \n";
        showHand(house->name(), house->handPtr());
        while (house->wantsCards(*active.front())) {
            std::cout << "Dealer hits.\n";
            house->takeCards(*deck);
            showHand(house->name(), house->handPtr());
        }
        int dealerVal = handValue(house->handPtr());

        
        std::cout << "\n            Results              \n";
        for (auto* p : active)
        {
            int playerVal = handValue(p->handPtr());
            int bet       = p->wager();          // accessor that returns `bet`
            int delta     = 0;                       // chips returned to bankroll

            bool naturalBJ = (playerVal == 21 && p->handPtr()->size() == 2);
            bool push      = (playerVal == dealerVal);

            if (playerVal > 21)
            {
                delta = 0;                           /* bust – stake already gone */
            }
            else if (naturalBJ && dealerVal != 21)
            {
                delta = bet * 5 / 2;                 /* stake + 1.5× profit */
            }
            else if (dealerVal > 21 || playerVal > dealerVal)
            {
                delta = bet * 2;                     /* stake + 1× profit */
            }
            else if (push)
            {
                delta = bet;                         /* stake returned */
            }
            else
            {
                delta = 0;                           /* dealer wins – nothing back */
            }

            p->settle(delta);                        /* add chips, reset bet to 0 */
            int net = delta - bet;                   /* profit (+) or loss (-) */

            std::cout << p->name() << ": "
                      << playerVal << " vs Dealer " << dealerVal << "   "
                      << (playerVal > 21 ? "BUST " :
                          net > 0       ? "WIN  " :
                          net == 0      ? "PUSH " :
                                          "LOSE ")
                      << (net >= 0 ? "+" : "") << net
                      << "   (bankroll " << p->credits() << ")\n";
        }

        delete deck;
        playMore = askYesNo("\nPlay another round");
        std::cout << '\n';
    }

    for (auto* p : players) delete p;
}
