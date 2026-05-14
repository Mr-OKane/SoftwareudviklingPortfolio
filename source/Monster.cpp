#include "Monster.h"
#include "Item.h"
#include "Status.h"
#include <iostream>

Monster::Monster(std::string name, int health, int strength): name(name), health(health), strength(strength), isDefeated(false), maxHealth(health) {}

void Monster::attack(Monster& target) {
    target.takeDamage(strength);
}

void Monster::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        isDefeated = true;
    }
}

int Monster::getHealth() const {
    return health;
}

int Monster::getMaxHealth() const {
    return maxHealth;
}

int Monster::getStrength() const {
    return strength;
}

std::string Monster::getName() const {
    return name;
}

bool Monster::getIsDefeated() const {
    return isDefeated;
}

void Monster::addItem(Item item) {
    items.push_back(item);
}

void Monster::removeItem(std::string itemName) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->getName() == itemName) {
            items.erase(it);
            return;
        }
    }
}

void Monster::addStatus(Status status) {
    statusEffects.push_back(status);
}

void Monster::executeStatusEffects() {
    for (auto& status : statusEffects) {
        status.executeTurn(*this);
    }
    // Fjern inaktive status-effekter
    statusEffects.erase(
        std::remove_if(statusEffects.begin(), statusEffects.end(),
            [](const Status& s) { return !s.isActive(); }),
        statusEffects.end()
    );
}

std::vector<Item>& Monster::getItems() {
    return items;
}

std::vector<Status>& Monster::getStatusEffects() {
    return statusEffects;
}

void Monster::resetStats() {
    health = maxHealth;
    isDefeated = false;
    statusEffects.clear();
}

Monster::~Monster() {}