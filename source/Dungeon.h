#ifndef DUNGEON_H
#define DUNGEON_H

#include "Monster.h"
#include "Item.h"
#include <vector>
#include <string>

class Dungeon {
private:
    std::string name;
    std::vector<Monster> monsters;
    Item rewardItem;
    int difficulty;  // 1-5 baseret på spillerens monstre
    
public:
    Dungeon(std::string name, int difficulty);
    
    std::string getName() const;
    std::vector<Monster>& getMonsters();
    Item getRewardItem() const;
    int getDifficulty() const;
    
    void generateMonsters();
    
    ~Dungeon();
};

#endif
