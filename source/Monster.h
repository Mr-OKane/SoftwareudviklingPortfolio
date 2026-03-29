#include <string>

class Monster {
    private:
    
        std::string name;
        int health;
        int strength;
        bool isDefeated;
        
    public:
    
        Monster(std::string name, int health, int strength);
        
        void attack(Monster& target);

        int getHealth();
        int getStrength();
        std::string getName();
        bool getIsDefeated();
        void takeDamage(int damage);
        
        ~Monster();
};