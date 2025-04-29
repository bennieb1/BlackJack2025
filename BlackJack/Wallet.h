#pragma once
struct Wallet {
    int credits = 1'000;
    bool withdraw(int x){ if(credits<x) return false; credits-=x; return true; }
    void deposit (int x){ credits+=x; }
};
