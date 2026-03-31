#include "Battle.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Monster.h"

Battle::Battle(Monster* player, Monster* enemy) 
    : playerMonster(player), enemyMonster(enemy), battleOver(false), playerFirst(false) {
    srand(time(0));
}

void Battle::battle() {
    bool firstMonster;
    firstMonster = diceRoll();
    while(battleOver != true) {
        if (firstMonster) {
            executeTurn(playerMonster,enemyMonster);
        }
        else {
            executeTurn(enemyMonster,playerMonster);
        }
        // Change turn
        firstMonster = !firstMonster;
    }
}

void Battle::executeTurn(Monster* attacker, Monster* defender) {
    attacker->attack(*defender);
    std::cout << attacker->getName() << " attacked " << defender->getName() << std::endl;
    std::cout << defender->getName() << " has " << defender->getHealth() << " HP left" << std::endl;
    if (defender->getIsDefeated()) {
        battleOver = true;
    }

}

bool Battle::diceRoll() {
    return rand() % 2 == 1;
}

Battle::~Battle() {}