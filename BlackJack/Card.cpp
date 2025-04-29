#include "Card.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <random>


char suitToChar(Suit s){
    switch(s){
        case Suit::Spades:   return 'S';
        case Suit::Hearts:   return 'H';
        case Suit::Diamonds: return 'D';
        case Suit::Clubs:    return 'C';
    }
    return '?';
}


static const std::array<Rank,13> ranks = {
    Rank::Ace,Rank::Two,Rank::Three,Rank::Four,Rank::Five,Rank::Six,
    Rank::Seven,Rank::Eight,Rank::Nine,Rank::Ten,
    Rank::Jack,Rank::Queen,Rank::King };

static const std::array<Suit,4> suits = {
    Suit::Spades,Suit::Hearts,Suit::Diamonds,Suit::Clubs };

static int rVal(Rank r){ return static_cast<int>(r); }


std::ostream& operator<<(std::ostream& os,const Card& c){
    static const char* face[]={"?","A","2","3","4","5","6","7","8","9","T","J","Q","K"};
    os<<face[rVal(c.rank)]<<suitToChar(c.suit);
    return os;
}


static std::minstd_rand rng(
    static_cast<std::minstd_rand::result_type>(
        std::chrono::steady_clock::now().time_since_epoch().count()) );


Deck* makeShuffledDeck(){
    Deck* d = new Deck;
    for(auto s: suits) for(auto r: ranks) d->push_back({r,s});
    std::shuffle(d->begin(),d->end(),rng);
    return d;
}


int handValue(const Hand* h){
    if(!h) return 0;
    int total=0, aces=0;
    for(const auto& c:*h){
        int v=rVal(c.rank);
        if(v==1){ total+=11; ++aces; }
        else if(v>=10) total+=10;
        else total+=v;
    }
    while(total>21 && aces--) total-=10;
    return total;
}


void showHand(const std::string& label,const Hand* h,bool hideFirst){
    std::cout<<label<<": ";
    if(!h){ std::cout<<"(empty)\n"; return; }
    for(size_t i=0;i<h->size();++i){
        if(hideFirst&&i==0) std::cout<<"?? ";
        else std::cout<<(*h)[i]<<' ';
    }
    if(!hideFirst) std::cout<<"(total = "<<handValue(h)<<')';
    std::cout<<'\n';
}
