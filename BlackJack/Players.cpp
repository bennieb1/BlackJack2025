#include "Players.h"

bool Players::placeBet()
{
    if (credit<=0)
    {
        std::cout << label << "is out of chips. \n";
        return false;
    }

    std::cout << label << " credits: " << credit << ". Enter bet (1-" << credit << "):";

    int wager = 0;
    std::cin >> wager;
    if (!std::cin || wager < 1 || wager > credit)
    {
        std::cout << "Invalid bet. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    bet = wager;
    credit -= wager;
    return true;
}


void Players::settle(int delta)
{
    credit += delta;
    bet = 0;
}




bool Players::wantsCards(const Participant& dealer)
{
    showHand(label, handPtr(), /*hideFirst=*/false);
    showHand(dealer.name(), dealer.handPtr(), /*hideFirst=*/true);

    /* 2) menu text ------------------------------------------------ */
    bool splittable = canSplit(*handPtr());

    std::cout << "[" << (splittable ? 'P' : ' ') << "H]Hit  [S]tand  "
              << (splittable ? "[P]Split  " : "          ")
              << "[D]ouble  [X]Pass  : ";

    char c; std::cin >> c;

    switch (std::toupper(c))
    {
    case 'P':
        if (!splittable) {
            std::cout << "Cannot split this hand.\n";
            return true;          // treat as Hit fallback
        }
        std::cout << "(Split not implemented in stub)\n";
        return true;
    case 'H':  return true;                       
    case 'D':
        if (credit >= bet) {               
            credit -= bet;          
            bet *= 2;
            std::cout << "Double down! Bet is now " <<bet << ".\n";
            return true;         
        }
        std::cout << "Not enough credits to double.\n";
        return true;             
    case 'X':  std::cout << "(Pass – you sit out this card)\n";  return false;
    case 'S':
    default:   return false;      
    }
}