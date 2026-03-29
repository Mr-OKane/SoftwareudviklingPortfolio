#ifndef PLAYER_H
#define PLAYER_H

#include "Monster.h"
#include <string>
#include <vector>

class Player {
    private:

        std::string name;
        std::vector<Monster> playerMonsters;
        
    public:

        Player(std::string name);
        std::string getPlayerName();
        void addMonster(Monster& target);
        void replaceMonster(Monster& target);
        std::vector<Monster>& getMonsters();
        bool allDefeated();
        ~Player();

};
#endif