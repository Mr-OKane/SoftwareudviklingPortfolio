#include "Dungeon.h"
#include <cstdlib>
#include <ctime>

Dungeon::Dungeon(std::string name, int difficulty) 
    : name(name), difficulty(difficulty) {
    generateMonsters();
    
    // Generate a random reward item for defeating the dungeon
    rewardItem = generateRandomItem();
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
    std::vector<std::string> monsterNames = {"Rattata", "Pidgeot", "Arcanine", "Lapras", "Dragonite"};
    
    int numMonsters = 2 + difficulty;  
    
    for (int i = 0; i < numMonsters; ++i) {
        std::string name = monsterNames[rand() % monsterNames.size()];
        int baseHealth = 50 + (difficulty * 15);  
        int baseStrength = 8 + (difficulty * 3);
        
        // Tilføj noget randomness
        int health = baseHealth + (rand() % 20 - 10);
        int strength = baseStrength + (rand() % 8 - 4);
        
        Monster dungeonMonster(name, health, strength);
        monsters.push_back(dungeonMonster);
    }
}

Dungeon::~Dungeon() {}
