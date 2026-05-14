#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include <vector>
#include <algorithm>
#include "Item.h"
#include "Status.h"

class Monster {
    private:
        std::string name;
        int health;
        int strength;
        bool isDefeated;
        int maxHealth;
        std::vector<Item> items;
        std::vector<Status> statusEffects;
        
    public:
        Monster(std::string name, int health, int strength);
        
        void attack(Monster& target);

        int getHealth() const;
        int getMaxHealth() const;
        int getStrength() const;
        std::string getName() const;
        bool getIsDefeated() const;
        void takeDamage(int damage);
        void addItem(Item item);
        void removeItem(std::string itemName);
        void addStatus(Status status);
        void executeStatusEffects();
        std::vector<Item>& getItems();
        std::vector<Status>& getStatusEffects();
        void resetStats();
        ~Monster();
};

#endif