#include "Game.h"
#include "Battle.h"

Game::Game() : player(nullptr), lastEnemyIndex(-1) {
    enemyMonsters.push_back(Monster("Bulbasaur", 80, 10));
    enemyMonsters.push_back(Monster("Squirtle", 70, 12));
    enemyMonsters.push_back(Monster("Geodude", 90, 8));
}

void Game::mainMenu() {
    int choice;

    do {
        std::cout << "\n===== MONSTER GAME =====" << std::endl;
        std::cout << "1. Create New Character" << std::endl;
        std::cout << "2. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1:
                createNewCharacter();
                break;
            case 2:
                std::cout << "Thanks for playing!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    while (choice != 2);
}

void Game::createNewCharacter() {
    if (player != nullptr) {
        delete player;
        player = nullptr;
    }
    newGame();
}

void Game::newGame() {
    std::string playerName;
    std::cout << "\nEnter your name: ";
    std::getline(std::cin, playerName);
    player = new Player(playerName);
    Monster starter("Pikachu", 60, 20);
    player->addMonster(starter);
    std::cout << "Welcome, " << player->getPlayerName() << "! Your adventure begins now!" << std::endl;
    std::cout << "You have received a starter monster: " << starter.getName() << "!" << std::endl;
    showMenu();
}

void Game::showMenu() {
    int choice;

    do {
        std::cout << "\n===== MAIN MENU =====" << std::endl;
        std::cout << "1. Adventure" << std::endl;
        std::cout << "2. Create New Character" << std::endl;
        std::cout << "3. Exit Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1:
                showAdventureMenu();
                break;
            case 2:
                createNewCharacter();
                break;
            case 3:
                std::cout << "Returning to main menu..." << std::endl;
                return;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    while (choice != 3);
} 


void Game::showAdventureMenu() {
    int choice;
    do {
        std::cout << "Adventure Menu: " << std::endl;
        std::cout << "1. Battle" << std::endl;
        std::cout << "2. Exit to Main Menu" << std::endl;
        std::cin >> choice;
        switch (choice) {
            case 1: {
                if (player->allDefeated()) {
                    std::cout << "All your monsters are defeated! Returning to main menu." << std::endl;
                    return;
                }

                if (enemyMonsters.empty()) {
                    std::cout << "No more enemies to battle! Returning to main menu." << std::endl;
                    return;
                }
                
                Monster* enemy = chooseEnemy();
                
                if (enemy == nullptr) { 
                    break; 
                }
                Monster* myMonster = choosePlayerMonster();
                
                if (myMonster == nullptr) { 
                    break; 
                }
                
                Battle battle(myMonster, enemy);
                battle.battle();
                
                if (myMonster->getIsDefeated()) {
                    std::cout << myMonster->getName() << " was defeated!" << std::endl;
                } 
                
                else {
                    std::cout << "You defeated " << enemy->getName() << "!" << std::endl;
                    captureMonster(*enemy);
                    enemyMonsters.erase(enemyMonsters.begin() + lastEnemyIndex);
                }
                break;
    }

}
    }while (choice != 2);
}


Monster* Game::chooseEnemy() {
    std::cout << "Choose an enemy to battle: " << std::endl;
    for (size_t i = 0; i < enemyMonsters.size(); ++i) {
        std::cout << i + 1 << ". " << enemyMonsters[i].getName() << " (HP: " << enemyMonsters[i].getHealth() << ", Strength: " << enemyMonsters[i].getStrength() << ")" << std::endl;
    }
    int choice;
    std::cin >> choice;

    if (choice < 1 || choice > (int)enemyMonsters.size()) {
        return nullptr;
    }

    lastEnemyIndex = choice - 1;
    return &enemyMonsters[lastEnemyIndex];
}

Monster* Game::choosePlayerMonster() {
    std::cout << "Choose a monster to battle with: " << std::endl;
    std::vector<Monster>& monsters = player->getMonsters();
    
    for (size_t i = 0; i < monsters.size(); ++i) {
        if (monsters[i].getIsDefeated()) {
            std::cout << i + 1 << ". " << monsters[i].getName() << " (DEFEATED)" << std::endl;
        } else {
        std::cout << i + 1 << ". " << monsters[i].getName() << " (HP: " << monsters[i].getHealth() << ", Strength: " << monsters[i].getStrength() << ")" << std::endl;
        }
    }

    int choice;
    std::cin >> choice;

    if (choice < 1 || choice > (int)monsters.size() || monsters[choice-1].getIsDefeated()) {
    return nullptr;
    }

    return &monsters[choice - 1];
}

void Game::captureMonster(Monster& enemy) {
    std::cout << "Do you want to capture " << enemy.getName() << "? (y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        enemy.resetStats();
        if (player->getMonsters().size() < 4) {
            player->addMonster(enemy);
        } else {
            player->replaceMonster(enemy);
        }
        std::cout << "You captured " << enemy.getName() << "!" << std::endl;
    }
}

Game::~Game() {
    delete player;
}