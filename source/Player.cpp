#include "Player.h"
#include <iostream>

Player::Player(std::string name) : name(name) {}

std::string Player::getPlayerName() {
    return name;
}

void Player::addMonster(Monster& target) {
    if (playerMonsters.size() < 4) {
        playerMonsters.push_back(target);
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
        playerMonsters[monsterIndex - 1] = target;
    }
    else {
        std::cout << "Invalid index" << std::endl;
    }
}

std::vector<Monster>& Player::getMonsters() {
    return playerMonsters;
}

void Player::addItem(Item item) {
    inventory.push_back(item);
}

void Player::removeItem(std::string itemName) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->getName() == itemName) {
            inventory.erase(it);
            return;
        }
    }
}

std::vector<Item>& Player::getInventory() {
    return inventory;
}

bool Player::allDefeated() {
    for(int i = 0; i < playerMonsters.size(); i++) {
        if (!playerMonsters[i].getIsDefeated()) {
            return false; 
        }
    }
    return true;
}

Player::~Player() {};