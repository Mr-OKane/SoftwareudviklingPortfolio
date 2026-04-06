#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Monster.h"
#include <vector>
#include <iostream>
#include <string>
class Game {
public:
    Game();
    void newGame();
    ~Game();

private:
    Player* player;

    std::vector<Monster> enemyMonsters;
    int lastEnemyIndex;
    
    void showMenu();
    void showAdventureMenu();
    Monster* chooseEnemy();
    Monster* choosePlayerMonster();
    void captureMonster(Monster& enemy);


};
#endif