#include "Dungeon.h"
#include <cstdlib>
#include <ctime>

Dungeon::Dungeon(std::string name, int difficulty) 
    : name(name), difficulty(difficulty) {
    
    // Generér dungeons monstre
    generateMonsters();
    
    // Generér en reward item baseret på difficulty
    std::vector<std::string> rewards = {"poison", "burn", "paralysis"};
    rewardItem = Item("Mystery Item", "A mysterious item that applies status effect", {rewards[rand() % rewards.size()]}, false, 10);
}

std::string Dungeon::getName() const {
    return name;
}

std::vector<Monster>& Dungeon::getMonsters() {
    return monsters;
}

Item Dungeon::getRewardItem() const {
    return rewardItem;
}

int Dungeon::getDifficulty() const {
    return difficulty;
}

void Dungeon::generateMonsters() {
    // Basis monstre baseret på difficulty
    std::vector<std::string> monsterNames = {"Rattata", "Pidgeot", "Arcanine", "Lapras", "Dragonite"};
    
    int numMonsters = 2 + difficulty;  // 3-7 monstre baseret på difficulty
    
    for (int i = 0; i < numMonsters; ++i) {
        std::string name = monsterNames[rand() % monsterNames.size()];
        int baseHealth = 50 + (difficulty * 15);  // Health mellem 65-125
        int baseStrength = 8 + (difficulty * 3);  // Strength mellem 11-23
        
        // Tilføj noget randomness
        int health = baseHealth + (rand() % 20 - 10);
        int strength = baseStrength + (rand() % 8 - 4);
        
        Monster dungeonMonster(name, health, strength);
        monsters.push_back(dungeonMonster);
    }
}

Dungeon::~Dungeon() {}
