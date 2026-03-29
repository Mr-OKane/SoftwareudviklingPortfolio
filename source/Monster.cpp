#include "Monster.h"

Monster::Monster(std::string name, int health, int strength) : name(name), health(health), strength(strength), isDefeated(false) {}

void Monster::attack(Monster& target) {
    target.takeDamage(strength);
}

void Monster::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        isDefeated = true;
    }
}

int Monster::getHealth() {
    return health;
}
int Monster::getStrength() {
    return strength;
}

std::string Monster::getName() {
    return name;
}
bool Monster::getIsDefeated() {
    return isDefeated;
}

Monster::~Monster() {}; 