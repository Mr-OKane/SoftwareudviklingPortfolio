#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>

class Item {
    private:
        std::string name;
        std::string description;
        std::vector<std::string> statusEffects;
        bool targetIsUser;      // true = selv, false = modstander
        int effectPower;        // Styrken af effekten

    public:
        Item();
        Item(std::string name, std::string description, std::vector<std::string> statusEffects);
        Item(std::string name, std::string description, std::vector<std::string> statusEffects, bool user, int power);
        
        std::string getName();
        std::string getDescription();
        std::vector<std::string> getStatusEffects();
        bool doesTargetUser() const;
        int getEffectPower() const;
        
        ~Item();
};

Item generateRandomItem();

#endif