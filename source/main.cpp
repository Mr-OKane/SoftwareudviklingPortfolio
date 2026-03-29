#include <iostream>
#include "Monster.h"
#include "Player.h"

int main() {
    Player player("Emil");
    Monster charmander("Charmander", 60, 15);
    Monster pikachu("Pikachu", 50, 20);
    
    player.addMonster(charmander);
    player.addMonster(pikachu);
    for (Monster& m : player.getMonsters()) {
        std::cout << m.getName() << " HP: " << m.getHealth() << std::endl;
    }
    
    
    return 0;
}