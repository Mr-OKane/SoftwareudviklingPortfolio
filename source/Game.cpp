#include "Game.h"
#include "Battle.h"
#include "Status.h"
#include <cstdlib>
#include <ctime>
#include <numeric>

Game::Game() : player(nullptr), lastEnemyIndex(-1), playerAverageLevel(1), currentHeroId(-1) {
    srand(time(0));
    enemyMonsters.push_back(Monster("Bulbasaur", 80, 10));
    enemyMonsters.push_back(Monster("Squirtle", 70, 12));
    enemyMonsters.push_back(Monster("Geodude", 90, 8));
}

void Game::mainMenu() {
    if (!database.open("monster_game.db")) {
        std::cerr << "Warning: database could not be opened. Save/load disabled." << std::endl;
    }

    int choice;

    do {
        std::cout << "\n===== MONSTER GAME =====" << std::endl;
        std::cout << "1. Load Saved Game" << std::endl;
        std::cout << "2. Create New Character" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1:
                showLoadMenu();
                break;
            case 2:
                createNewCharacter();
                break;
            case 3:
                std::cout << "Thanks for playing!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    while (choice != 3);
}

void Game::createNewCharacter() {
    if (player != nullptr) {
        delete player;
        player = nullptr;
    }
    currentHeroId = -1;
    newGame();
}

void Game::showLoadMenu() {
    std::vector<HeroSummary> heroes = database.listHeroes();

    if (heroes.empty()) {
        std::cout << "No saved heroes found. Create a new character instead." << std::endl;
        return;
    }

    std::cout << "\n===== LOAD SAVED GAME =====" << std::endl;
    for (size_t i = 0; i < heroes.size(); ++i) {
        std::cout << i + 1 << ". " << heroes[i].name << std::endl;
    }
    std::cout << heroes.size() + 1 << ". Back" << std::endl;

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice == (int)heroes.size() + 1) {
        return;
    }
    if (choice < 1 || choice > (int)heroes.size()) {
        std::cout << "Invalid choice." << std::endl;
        return;
    }

    int heroId = heroes[choice - 1].id;
    if (player != nullptr) {
        delete player;
        player = nullptr;
    }

    if (!database.loadHero(heroId, player)) {
        std::cout << "Failed to load saved hero." << std::endl;
        return;
    }

    currentHeroId = heroId;
    generateDungeons();
    std::cout << "Welcome back, " << player->getPlayerName() << "!" << std::endl;
    showMenu();
}

void Game::saveGame() {
    if (player == nullptr) {
        std::cout << "No active game to save." << std::endl;
        return;
    }

    if (!database.saveHero(*player, currentHeroId)) {
        std::cout << "Unable to save the game." << std::endl;
        return;
    }

    std::cout << "Game saved successfully." << std::endl;
}

void Game::showStatistics() {
    if (player == nullptr) {
        std::cout << "No active character. Start or load a game first." << std::endl;
        return;
    }

    std::vector<Monster>& monsters = player->getMonsters();
    std::vector<Item>& inventory = player->getInventory();

    int totalMonsters = static_cast<int>(monsters.size());
    int defeatedCount = 0;
    int totalHealth = 0;
    int totalStrength = 0;
    int activeMonsters = 0;

    for (const Monster& monster : monsters) {
        if (monster.getIsDefeated()) {
            defeatedCount++;
        } else {
            activeMonsters++;
        }
        totalHealth += monster.getHealth();
        totalStrength += monster.getStrength();
    }

    double averageHealth = totalMonsters > 0 ? static_cast<double>(totalHealth) / totalMonsters : 0.0;
    double averageStrength = totalMonsters > 0 ? static_cast<double>(totalStrength) / totalMonsters : 0.0;

    std::cout << "\n===== PLAYER STATISTICS =====" << std::endl;
    std::cout << "Name: " << player->getPlayerName() << std::endl;
    std::cout << "Monsters owned: " << totalMonsters << std::endl;
    std::cout << "Active monsters: " << activeMonsters << std::endl;
    std::cout << "Defeated monsters: " << defeatedCount << std::endl;
    std::cout << "Total monster HP: " << totalHealth << std::endl;
    std::cout << "Average monster HP: " << static_cast<int>(averageHealth) << std::endl;
    std::cout << "Average monster strength: " << static_cast<int>(averageStrength) << std::endl;
    std::cout << "Inventory size: " << inventory.size() << std::endl;

    std::map<std::string, int> itemCounts;
    for (const Item& item : inventory) {
        itemCounts[item.getName()]++;
    }

    if (!itemCounts.empty()) {
        std::cout << "Inventory items:" << std::endl;
        for (const auto& entry : itemCounts) {
            std::cout << " - " << entry.first << ": " << entry.second << std::endl;
        }
    } else {
        std::cout << "Inventory items: None" << std::endl;
    }
}

void Game::newGame() {
    std::string playerName;
    std::cout << "\nEnter your name: ";
    std::getline(std::cin, playerName);
    player = new Player(playerName);
    Monster starter("Pikachu", 60, 20);
    
    // Add starting items to the player's inventory
    Item smallPotion("Small Healing Potion", "Heals 25 HP", {}, true, 25);
    Item mediumPotion("Medium Healing Potion", "Heals 50 HP", {}, true, 50);
    player->addItem(smallPotion);
    player->addItem(smallPotion);
    player->addItem(mediumPotion);
    
    player->addMonster(starter);
    
    // Generér grotter
    generateDungeons();
    
    std::cout << "Welcome, " << player->getPlayerName() << "! Your adventure begins now!" << std::endl;
    std::cout << "You have received a starter monster: " << starter.getName() << "!" << std::endl;
    std::cout << "Your inventory contains: 2x Small Healing Potion and 1x Medium Healing Potion!" << std::endl;
    showMenu();
}

void Game::showMenu() {
    int choice;

    do {
        std::cout << "\n===== MAIN MENU =====" << std::endl;
        std::cout << "1. Adventure" << std::endl;
        std::cout << "2. Dungeons" << std::endl;
        std::cout << "3. View Inventory" << std::endl;
        std::cout << "4. Statistics" << std::endl;
        std::cout << "5. Save Game" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cin >> choice;
        std::cin.ignore();  // Clear newline from buffer
        
        switch (choice) {
            case 1:
                showAdventureMenu();
                break;
            case 2:
                showDungeonMenu();
                break;
            case 3:
                useItemFromInventory();
                break;
            case 4:
                showStatistics();
                break;
            case 5:
                saveGame();
                break;
            case 6:
                std::cout << "Thanks for playing!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    while (choice != 6);
}

void Game::showAdventureMenu() {
    int choice;
    do {
        std::cout << "\n===== ADVENTURE MENU =====" << std::endl;
        std::cout << "1. Battle" << std::endl;
        std::cout << "2. Back to Main Menu" << std::endl;
        std::cin >> choice;
        std::cin.ignore();
        
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
            case 2:
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    } while (choice != 2);
}

void Game::showDungeonMenu() {
    int choice;
    do {
        std::cout << "\n===== DUNGEON MENU =====" << std::endl;
        for (size_t i = 0; i < dungeons.size(); ++i) {
            std::cout << i + 1 << ". " << dungeons[i].getName() << " (Difficulty: " << dungeons[i].getDifficulty() << ")" << std::endl;
        }
        std::cout << dungeons.size() + 1 << ". Back to Main Menu" << std::endl;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == (int)dungeons.size() + 1) {
            break;
        }
        
        if (choice < 1 || choice > (int)dungeons.size()) {
            std::cout << "Invalid choice." << std::endl;
            continue;
        }
        
        // Start dungeon battle
        Dungeon& selectedDungeon = dungeons[choice - 1];
        std::cout << "\nEntering " << selectedDungeon.getName() << "..." << std::endl;
        
        if (player->allDefeated()) {
            std::cout << "All your monsters are defeated! Cannot enter dungeon." << std::endl;
            continue;
        }
        
        // Battle alle monstre i grotten
        std::vector<Monster>& dungeonMonsters = selectedDungeon.getMonsters();
        bool dungeonCompleted = true;
        
        for (auto& dungeonMonster : dungeonMonsters) {
            if (player->allDefeated()) {
                std::cout << "All your monsters are defeated! Dungeon incomplete." << std::endl;
                dungeonCompleted = false;
                break;
            }
            
            Monster* playerMonster = choosePlayerMonster();
            if (playerMonster == nullptr) {
                dungeonCompleted = false;
                break;
            }
            
            std::cout << "\nBattle against " << dungeonMonster.getName() << "!" << std::endl;
            Battle battle(playerMonster, &dungeonMonster);
            battle.battle();
            
            if (playerMonster->getIsDefeated()) {
                std::cout << playerMonster->getName() << " was defeated!" << std::endl;
            } else {
                std::cout << "You defeated " << dungeonMonster.getName() << "!" << std::endl;
            }
        }
        
        if (dungeonCompleted) {
            completeDungeon(selectedDungeon);
        }
        
    } while (choice != (int)dungeons.size() + 1);
}

void Game::completeDungeon(Dungeon& dungeon) {
    std::cout << "\n*** DUNGEON COMPLETE! ***" << std::endl;
    Item reward = dungeon.getRewardItem();
    std::cout << "You received: " << reward.getName() << "!" << std::endl;
    player->addItem(reward);
    
    // Giv spilleren option til at give item til et monster
    giveItemToMonster(reward);
    
    // Regenerer grotten
    dungeons.erase(
        std::remove_if(dungeons.begin(), dungeons.end(),
            [&dungeon](const Dungeon& d) { return d.getName() == dungeon.getName(); }),
        dungeons.end()
    );
    generateDungeons();
}

void Game::giveItemToMonster(Item item) {
    std::cout << "\nDo you want to give this item to one of your monsters? (y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice != 'y' && choice != 'Y') {
        return;
    }
    
    std::vector<Monster>& monsters = player->getMonsters();
    std::cout << "Choose a monster:" << std::endl;
    for (size_t i = 0; i < monsters.size(); ++i) {
        std::cout << i + 1 << ". " << monsters[i].getName() << std::endl;
    }
    
    int monsterChoice;
    std::cin >> monsterChoice;
    std::cin.ignore();
    
    if (monsterChoice >= 1 && monsterChoice <= (int)monsters.size()) {
        monsters[monsterChoice - 1].addItem(item);
        std::cout << monsters[monsterChoice - 1].getName() << " received " << item.getName() << "!" << std::endl;
        player->removeItem(item.getName());
    }
}

void Game::useItemFromInventory() {
    std::cout << "\n===== INVENTORY =====" << std::endl;
    std::vector<Item>& inventory = player->getInventory();
    
    if (inventory.empty()) {
        std::cout << "Your inventory is empty!" << std::endl;
        return;
    }
    
    int itemChoice;
    do {
        std::cout << "\nAvailable items:" << std::endl;
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << i + 1 << ". " << inventory[i].getName() << " - " << inventory[i].getDescription() << std::endl;
        }
        std::cout << inventory.size() + 1 << ". Back" << std::endl;
        std::cin >> itemChoice;
        std::cin.ignore();
        
        if (itemChoice == (int)inventory.size() + 1) {
            return;
        }
        
        if (itemChoice < 1 || itemChoice > (int)inventory.size()) {
            std::cout << "Invalid choice. Please try again." << std::endl;
            continue;
        }
        
        // Show monsters to apply item to
        std::vector<Monster>& monsters = player->getMonsters();
        std::cout << "\nChoose a monster to use item on:" << std::endl;
        for (size_t i = 0; i < monsters.size(); ++i) {
            std::cout << i + 1 << ". " << monsters[i].getName() << " (HP: " << monsters[i].getHealth() << "/" << monsters[i].getMaxHealth() << ")" << std::endl;
        }
        
        int monsterChoice;
        std::cin >> monsterChoice;
        std::cin.ignore();
        
        if (monsterChoice < 1 || monsterChoice > (int)monsters.size()) {
            std::cout << "Invalid choice." << std::endl;
            continue;
        }
        
        Monster& targetMonster = monsters[monsterChoice - 1];
        Item& item = inventory[itemChoice - 1];
        
        // Check if it's a healing item (targetIsUser = true, no status effects)
        if (item.doesTargetUser() && item.getStatusEffects().empty()) {
            int healAmount = item.getEffectPower();
            targetMonster.heal(healAmount);
            std::cout << "\n" << targetMonster.getName() << " used " << item.getName() << " and healed " << healAmount << " HP!" << std::endl;
            std::cout << targetMonster.getName() << " now has " << targetMonster.getHealth() << " HP" << std::endl;
            player->removeItem(item.getName());
        }
        // Check if it's a damage item (targetIsUser = false, no status effects)
        else if (!item.doesTargetUser() && item.getStatusEffects().empty()) {
            std::cout << "\nThis item is for use in battle, not in the inventory." << std::endl;
        }
        // Otherwise it's a status effect item
        else {
            std::vector<std::string> effects = item.getStatusEffects();
            for (const auto& effect : effects) {
                targetMonster.addStatus(Status(effect, 3));
            }
            std::cout << "\n" << targetMonster.getName() << " used " << item.getName() << "!" << std::endl;
            player->removeItem(item.getName());
        }
        
        break;  // Exit after using item
    } while (true);
}

void Game::generateDungeons() {
    dungeons.clear();
    playerAverageLevel = calculatePlayerLevel();
    
    std::vector<std::string> dungeonNames = {"Forest Cave", "Mountain Peak", "Volcano Crater", "Ice Palace", "Dragon's Lair"};
    
    // Generér 3-5 grotter baseret på spillerens niveau
    int numDungeons = 3 + (playerAverageLevel / 3);
    if (numDungeons > 5) numDungeons = 5;
    
    for (int i = 0; i < numDungeons; ++i) {
        int difficulty = 1 + (rand() % playerAverageLevel);
        if (difficulty > 5) difficulty = 5;
        
        Dungeon dungeon(dungeonNames[i], difficulty);
        dungeons.push_back(dungeon);
    }
}

int Game::calculatePlayerLevel() {
    std::vector<Monster>& monsters = player->getMonsters();
    if (monsters.empty()) return 1;
    
    int totalStrength = 0;
    for (const auto& monster : monsters) {
        totalStrength += monster.getStrength();
    }
    
    return std::max(1, totalStrength / (int)monsters.size() / 5);
}

Monster* Game::chooseEnemy() {
    std::cout << "\nChoose an enemy to battle: " << std::endl;
    for (size_t i = 0; i < enemyMonsters.size(); ++i) {
        std::cout << i + 1 << ". " << enemyMonsters[i].getName() << " (HP: " << enemyMonsters[i].getHealth() << ", Strength: " << enemyMonsters[i].getStrength() << ")" << std::endl;
    }
    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice < 1 || choice > (int)enemyMonsters.size()) {
        return nullptr;
    }

    lastEnemyIndex = choice - 1;
    return &enemyMonsters[lastEnemyIndex];
}

Monster* Game::choosePlayerMonster() {
    std::cout << "\nChoose a monster to battle with: " << std::endl;
    std::vector<Monster>& monsters = player->getMonsters();
    
    for (size_t i = 0; i < monsters.size(); ++i) {
        if (monsters[i].getIsDefeated()) {
            std::cout << i + 1 << ". " << monsters[i].getName() << " (DEFEATED)" << std::endl;
        } 
        else {
            std::cout << i + 1 << ". " << monsters[i].getName() << " (HP: " << monsters[i].getHealth() << ", Strength: " << monsters[i].getStrength() << ")" << std::endl;
        }
    }

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice < 1 || choice > (int)monsters.size() || monsters[choice-1].getIsDefeated()) {
        return nullptr;
    }

    return &monsters[choice - 1];
}

void Game::captureMonster(Monster& enemy) {
    std::cout << "\nDo you want to capture " << enemy.getName() << "? (y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice == 'y' || choice == 'Y') {
        enemy.resetStats();
        if (player->getMonsters().size() < 4) {
            player->addMonster(enemy);
        } 
        else {
            player->replaceMonster(enemy);
        }
        std::cout << "You captured " << enemy.getName() << "!" << std::endl;
    }
}

Game::~Game() {
    delete player;
}