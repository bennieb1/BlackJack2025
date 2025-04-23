#include "Suits.h"

char toChar(Suits s)
{
    switch (s)
    {
    case Suits::Spades:   return 'S';
    case Suits::Hearts:   return 'H';
    case Suits::Diamonds: return 'D';
    case Suits::Clubs:    return 'C';
    }
    return '?';   // should never happen
}