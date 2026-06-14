#ifndef BATTLE_H
#define BATTLE_H

#include "Monster.h"
#include "Item.h"

enum class ActionType {
    ATTACK,
    USE_ITEM,
    SWITCH_MONSTER
};

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
    ActionType playerChooseAction();
    Item* playerChooseItem();
    bool isParalyzed(Monster* monster);
    ~Battle();
};

#endif