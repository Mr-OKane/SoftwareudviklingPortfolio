#include "Item.h"

Item::Item() : name(""), description(""), statusEffects({}), targetIsUser(false), effectPower(0) {

}

Item::Item(std::string name, std::string description, std::vector<std::string> statusEffects) 
    : name(name), description(description), statusEffects(statusEffects), targetIsUser(false), effectPower(0) {

}

Item::Item(std::string name, std::string description, std::vector<std::string> statusEffects, bool user, int power)
    : name(name), description(description), statusEffects(statusEffects), targetIsUser(user), effectPower(power) {

}

std::string Item::getName() {
    return name;
}

std::string Item::getDescription() {
    return description;
}   

std::vector<std::string> Item::getStatusEffects() {
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