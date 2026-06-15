#include "Item.h"
#include <cstdlib>
#include <ctime>

Item::Item() : name(""), description(""), statusEffects({}), targetIsUser(false), effectPower(0) {

}

Item::Item(std::string name, std::string description, std::vector<std::string> statusEffects) 
    : name(name), description(description), statusEffects(statusEffects), targetIsUser(false), effectPower(0) {

}

Item::Item(std::string name, std::string description, std::vector<std::string> statusEffects, bool user, int power)
    : name(name), description(description), statusEffects(statusEffects), targetIsUser(user), effectPower(power) {

}

std::string Item::getName() const {
    return name;
}

std::string Item::getDescription() const {
    return description;
}
   
std::vector<std::string> Item::getStatusEffects() const {
    return statusEffects;
}

bool Item::doesTargetUser() const {
    return targetIsUser;
}

int Item::getEffectPower() const {
    return effectPower;
}

Item::~Item() {

}

Item generateRandomItem() {
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }
    
    int itemType = rand() % 7;  // 7 different items
    
    switch(itemType) {
        case 0: 
            return Item("Small Healing Potion", "Heals 25 HP", {}, true, 25);
        case 1: 
            return Item("Medium Healing Potion", "Heals 50 HP", {}, true, 50);
        case 2:  
            return Item("Large Healing Potion", "Heals 75 HP", {}, true, 75);
        case 3:  
            return Item("Bomb", "Deals 25 damage", {}, false, 25);
        case 4:  
            return Item("Taser", "Paralyzes opponent - they miss next turn", {"paralysis"}, false, 0);
        case 5:  
            return Item("Poison Dart", "Poisons opponent - 5 damage per round", {"poison"}, false, 0);
        case 6:  
            return Item("Flamethrower", "Burns opponent - deals damage each turn", {"burn"}, false, 0);
        default:
            return Item("Small Healing Potion", "Heals 25 HP", {}, true, 25);
    }
}