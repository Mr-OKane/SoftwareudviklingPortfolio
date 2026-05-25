#include "Battle.h"
#include "Status.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Monster.h"

Battle::Battle(Monster* player, Monster* enemy) 
    : playerMonster(player), enemyMonster(enemy), battleOver(false), playerFirst(false) {
    srand(time(0));
}

void Battle::battle() {
    bool firstMonster = diceRoll();
    std::cout << (firstMonster ? playerMonster->getName() : enemyMonster->getName()) << " goes first!" << std::endl;
    
    while(!battleOver) {
        if (firstMonster) {
            std::cout << "\n--- " << playerMonster->getName() << "'s Turn ---" << std::endl;
            
            // Udføre status-effekter
            playerMonster->executeStatusEffects();
            
            // Check for paralysis
            if (isParalyzed(playerMonster)) {
                std::cout << playerMonster->getName() << " is paralyzed and cannot move!" << std::endl;
            } else {
                // Spilleren vælger handling
                while (true) {
                    ActionType action = playerChooseAction();
                    
                    if (action == ActionType::ATTACK) {
                        executeTurn(playerMonster, enemyMonster);
                        break;
                    }
                    else if (action == ActionType::USE_ITEM) {
                        Item* item = playerChooseItem();
                        if (item == nullptr) {
                            std::cout << "Return to action menu." << std::endl;
                            continue;
                        }
                        
                        std::vector<std::string> effects = item->getStatusEffects();
                        
                        // Check if it's a healing item (targetIsUser = true, no status effects)
                        if (item->doesTargetUser() && effects.empty()) {
                            int healAmount = item->getEffectPower();
                            playerMonster->heal(healAmount);
                            std::cout << playerMonster->getName() << " used " << item->getName() << " and healed " << healAmount << " HP!" << std::endl;
                            std::cout << playerMonster->getName() << " now has " << playerMonster->getHealth() << " HP" << std::endl;
                        }
                        // Check if it's a damage item (targetIsUser = false, no status effects)
                        else if (!item->doesTargetUser() && effects.empty()) {
                            int damage = item->getEffectPower();
                            enemyMonster->takeDamage(damage);
                            std::cout << playerMonster->getName() << " used " << item->getName() << " on " << enemyMonster->getName() << " for " << damage << " damage!" << std::endl;
                            std::cout << enemyMonster->getName() << " now has " << enemyMonster->getHealth() << " HP" << std::endl;
                        }
                        // Otherwise it's a status effect item
                        else {
                            for (const auto& effect : effects) {
                                if (item->doesTargetUser()) {
                                    playerMonster->addStatus(Status(effect, 3));
                                    std::cout << playerMonster->getName() << " used " << item->getName() << "!" << std::endl;
                                } else {
                                    enemyMonster->addStatus(Status(effect, 3));
                                    std::cout << playerMonster->getName() << " used " << item->getName() << " on " << enemyMonster->getName() << "!" << std::endl;
                                }
                            }
                        }
                        break;
                    }
                }

            }
        }
        else {
            std::cout << "\n--- " << enemyMonster->getName() << "'s Turn ---" << std::endl;
            
            // Udføre status-effekter
            enemyMonster->executeStatusEffects();
            
            // Check for paralysis
            if (isParalyzed(enemyMonster)) {
                std::cout << enemyMonster->getName() << " is paralyzed and cannot move!" << std::endl;
            } else {
                executeTurn(enemyMonster, playerMonster);
            }
        }
        
        // Change turn
        firstMonster = !firstMonster;
        
        // Check hvis nogen er besejret
        if (playerMonster->getIsDefeated() || enemyMonster->getIsDefeated()) {
            battleOver = true;
        }
    }
}

void Battle::executeTurn(Monster* attacker, Monster* defender) {
    attacker->attack(*defender);
    std::cout << attacker->getName() << " attacked " << defender->getName() << std::endl;
    std::cout << defender->getName() << " has " << defender->getHealth() << " HP left" << std::endl;
}

ActionType Battle::playerChooseAction() {
    int choice;
    std::cout << "1. Attack" << std::endl;
    std::cout << "2. Use Item" << std::endl;
    std::cin >> choice;
    
    if (choice == 2) {
        return ActionType::USE_ITEM;
    }
    return ActionType::ATTACK;
}

Item* Battle::playerChooseItem() {
    std::vector<Item>& items = playerMonster->getItems();
    
    if (items.empty()) {
        std::cout << "You have no items!" << std::endl;
        return nullptr;
    }
    
    std::cout << "Choose an item:" << std::endl;
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << i + 1 << ". " << items[i].getName() << " - " << items[i].getDescription() << std::endl;
    }
    std::cout << items.size() + 1 << ". Back" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    // Back option
    if (choice == (int)items.size() + 1) {
        return nullptr;  // Return nullptr to signal back/cancel
    }
    
    if (choice < 1 || choice > (int)items.size()) {
        return nullptr;
    }
    
    return &items[choice - 1];
}

bool Battle::isParalyzed(Monster* monster) {
    std::vector<Status>& statuses = monster->getStatusEffects();
    for (const auto& status : statuses) {
        if (status.getType() == "paralysis") {
            // 30% chance at blive paralyzeret
            return (rand() % 100) < 30;
        }
    }
    return false;
}

bool Battle::diceRoll() {
    return rand() % 2 == 1;
}

Battle::~Battle() {}