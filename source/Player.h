#ifndef PLAYER_H
#define PLAYER_H

#include "Monster.h"
#include "Item.h"
#include <string>
#include <vector>

class Player {
    private:
        std::string name;
        std::vector<Monster> playerMonsters;
        std::vector<Item> inventory;
        
    public:
        Player(std::string name);
        std::string getPlayerName();
        void addMonster(Monster& target);
        void replaceMonster(Monster& target);
        std::vector<Monster>& getMonsters();
        void addItem(Item item);
        void removeItem(std::string itemName);
        std::vector<Item>& getInventory();
        bool allDefeated();
        ~Player();

};
#endif