#include "Card.h"



static const std::array<Ranks,13> allRanks = {

    Ranks::Ace, Ranks::Two, Ranks::Three, Ranks::Four, Ranks::Five, Ranks::Six, Ranks::Seven, Ranks::Eight,
    Ranks::Nine, Ranks::Ten, Ranks::Jack, Ranks::Queen, Ranks::King
    
};

static const std::array<Suits,4> allSuits = {
    Suits::Spades, Suits::Hearts, Suits::Diamonds, Suits::Clubs
};

static int cardValue(Ranks rank)
{
    switch (rank)
    {
    case Ranks::Ace: return 1;
        case Ranks::Two: return 2;
        case Ranks::Three: return 3;
        case Ranks::Four: return 4;
        case Ranks::Five: return 5;
        case Ranks::Six: return 6;
        case Ranks::Seven: return 7;
        case Ranks::Eight: return 8;
        case Ranks::Nine: return 9;
        case Ranks::Ten: return 10;
        case Ranks::Jack: return 11;
        case Ranks::Queen: return 12;
        case Ranks::King: return 13;
    }
    return 0;
}

std::ostream& operator<<(std::ostream& os, const Cards& card)
{

    static const char* ranksNames[] =
        {"","A","2","3","4","5","6","7","8","9","T","J","Q","K"};
    os << ranksNames[static_cast<int>(card.rank)] << toChar(card.suit);
    return os;
    
}


static std::minstd_rand makeEngine()
{
    auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    return std::minstd_rand{
        static_cast<std::minstd_rand::result_type>(now) };   // conversion OK
}
static std::minstd_rand rng = makeEngine();


Deck* makeShuffledDeck()
{
    Deck* d = new Deck;
    for (Suits s : allSuits)
        for (Ranks r : allRanks)
            d->push_back({ r, s });

    std::shuffle(d->begin(), d->end(), rng);
    return d;      
}



int handValue(const Hands* hand)
{
    if (!hand) return 0;

    int total = 0;
    int aceCount = 0;

    for (const Cards& c : *hand)
    {
        int rv = cardValue(c.rank);

        if (rv == 1) {           // Ace
            total += 11;
            ++aceCount;
        }
        else if (rv >= 10) {     // Ten / Face
            total += 10;
        }
        else {                   // 2-9
            total += rv;
        }
    }

    /* downgrade Aces 11→1 while busting */
    while (total > 21 && aceCount--)
        total -= 10;

    return total;
}


void showHand(const std::string& label, const Hands* hand, bool hideFirst)
{
    std::cout << label << ": ";
    if (!hand) { std::cout << "(empty)\n"; return; }

    for (size_t i = 0; i < hand->size(); ++i)
        if (hideFirst && i == 0) std::cout << "?? ";
        else                     std::cout << (*hand)[i] << ' ';

    if (!hideFirst)
        std::cout << "(total = " << handValue(hand) << ')';   

    std::cout << '\n';
}


