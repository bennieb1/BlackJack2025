#pragma once
#include "Rules.h"
struct State;


class Strategy {
public:
    virtual ~Strategy() = default;
    virtual int  bet (int bankroll) = 0;
    virtual bool hit (const State& self,const State& dealer,const TableRules&) = 0;
    virtual bool split(const State&,const TableRules&) = 0;
    virtual bool dbl (const State&,const TableRules&) = 0;
};

class ConsoleAI : public Strategy {
public:
    int  bet (int) override;
    bool hit (const State&,const State&,const TableRules&) override;
    bool split(const State&,const TableRules&) override;
    bool dbl (const State&,const TableRules&) override;
};

class DealerAI : public Strategy {
public:
    int  bet (int) override { return 0; }
    bool hit (const State&,const State&,const TableRules&) override;
    bool split(const State&,const TableRules&) override { return false; }
    bool dbl (const State&,const TableRules&) override { return false; }
};
