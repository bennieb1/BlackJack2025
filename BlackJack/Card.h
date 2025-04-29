#pragma once
#include <ostream>
#include <vector>
#include <string>


enum class Suit { Spades, Hearts, Diamonds, Clubs };
char suitToChar(Suit s);

enum class Rank : int {
    Ace = 1, Two, Three, Four, Five, Six, Seven,
    Eight, Nine, Ten, Jack, Queen, King
};
struct Card { Rank rank; Suit suit; };


using Deck = std::vector<Card>;
using Hand = std::vector<Card>;


std::ostream& operator<<(std::ostream&, const Card&);
Deck* makeShuffledDeck();            // caller must delete
int   handValue(const Hand*);        // Blackjack total (Ace soft 11)
void  showHand(const std::string& label,
               const Hand* h,
               bool hideFirst = false);
