#pragma once
#include <vector>
#include <ostream>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include<iostream>
#include <array>

#include "Suits.h"

enum class Ranks { Ace = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack,  Queen, King  };

struct Cards{Ranks rank; Suits suit;};

using Hands = std::vector<Cards>;
using Deck = std::vector<Cards>;

std::ostream& operator<<(std::ostream&, const Cards&);

Deck* makeShuffledDeck();

int handValue(const Hands* hand);

void showHand(const std::string& label, const Hands* hand, bool hideFirst = false);

