#include <iostream>
#include "Monster.h"

int main() {
    Monster charmander("Charmander", 60, 15);
    Monster pikachu("Pikachu", 50, 20);
    
    std::cout << pikachu.getHealth() << std::endl;
    charmander.attack(pikachu);
    std::cout << pikachu.getHealth() << std::endl;
    
    return 0;
}