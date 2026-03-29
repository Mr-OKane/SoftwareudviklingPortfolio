#include "Player.h"
#include <iostream>
Player::Player(std::string name) : name(name) {}

std::string Player::getPlayerName() {
    return name;
}

void Player::addMonster(Monster& target) {
    if (monsters.size() < 4) {
    monsters.push_back(target);
    }
    else {
        std::cout << "Cannot add more monsters. Limit reached! " << std::endl; 
    }
}

void Player::replaceMonster(Monster& target) {
    int monsterIndex;
    std::cout << "What monster do you want to replace? (1-4)" << std::endl;
    std::cin >> monsterIndex;
    if (monsterIndex >= 1 && monsterIndex <= 4) {
        monsters[monsterIndex - 1] = target;
    }
    else {
        std::cout << "Invalid index" << std::endl;
    }
    
}

std::vector<Monster>& Player::getMonsters() {
    return monsters;
}

bool Player::allDefeated() {
    for(int i = 0; i < monsters.size(); i++) {
        if (!monsters[i].getIsDefeated()) {
            return false; 
        }
    }
    return true; 

}