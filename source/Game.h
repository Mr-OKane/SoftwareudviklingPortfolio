#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Monster.h"
#include "Dungeon.h"
#include <vector>
#include <iostream>
#include <string>

class Game {
public:
    Game();
    void mainMenu();
    void newGame();
    ~Game();

private:
    Player* player;
    std::vector<Monster> enemyMonsters;
    std::vector<Dungeon> dungeons;
    int lastEnemyIndex;
    int playerAverageLevel;
    
    void createNewCharacter();
    void showMenu();
    void showAdventureMenu();
    void showDungeonMenu();
    void completeDungeon(Dungeon& dungeon);
    void giveItemToMonster(Item item);
    void useItemFromInventory();
    void generateDungeons();
    int calculatePlayerLevel();
    Monster* chooseEnemy();
    Monster* choosePlayerMonster();
    void captureMonster(Monster& enemy);
};

#endif