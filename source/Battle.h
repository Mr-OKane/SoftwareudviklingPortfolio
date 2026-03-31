#ifndef BATTLE_H
#define BATTLE_H

#include "Monster.h"

class Battle {

    private:
    
    bool battleOver;
    bool playerFirst;
    Monster* playerMonster;
    Monster* enemyMonster;
    
    public:
    Battle(Monster* player, Monster* enemy);
    void battle();
    bool diceRoll();
    void executeTurn(Monster* attacker, Monster* defender);
    ~Battle();
};

#endif