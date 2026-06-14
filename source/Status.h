#ifndef Status_H
#define Status_H

#include <string>

class Monster;  // Forward declaration

class Status {
private:
    std::string type;  // "poison", "paralysis", etc.
    int turnsRemaining;
    
public:
    Status(std::string type, int turns);
    void executeTurn(Monster& monster);  // Udføres ved start af tur
    bool isActive() const;
    std::string getType() const;
};

#endif