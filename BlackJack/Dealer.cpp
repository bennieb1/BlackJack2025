#include "Dealer.h"

bool Dealer::wantsCards(const Participant& )
{
    return handValue(handPtr()) < 17;
}
