#include "Status.h"
#include "Monster.h"
#include <iostream>

Status::Status(std::string type, int turns) : type(type), turnsRemaining(turns) {}

void Status::executeTurn(Monster& monster) {
    if (!isActive()) return;
    
    if (type == "poison") {
        int damage = monster.getMaxHealth() / 8;  // 12.5% af max HP
        monster.takeDamage(damage);
        std::cout << monster.getName() << " takes " << damage << " poison damage!" << std::endl;
    }
    else if (type == "burn") {
        int damage = monster.getMaxHealth() / 6;  // 16.7% af max HP
        monster.takeDamage(damage);
        std::cout << monster.getName() << " takes " << damage << " burn damage!" << std::endl;
    }
    else if (type == "paralysis") {
        // Paralysis bliver håndteret i Battle ved chance for at springe tur over
        std::cout << monster.getName() << " is paralyzed!" << std::endl;
    }
    
    turnsRemaining--;
}

bool Status::isActive() const {
    return turnsRemaining > 0;
}

std::string Status::getType() const {
    return type;
}