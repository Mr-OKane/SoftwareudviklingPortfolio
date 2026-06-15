#include "Database.h"
#include "Monster.h"
#include "Item.h"
#include <iostream>
#include <sstream>

DatabaseManager::DatabaseManager() : db(nullptr) {}

DatabaseManager::~DatabaseManager() {
    close();
}

bool DatabaseManager::open(const std::string& path) {
    if (sqlite3_open(path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return initializeSchema();
}

void DatabaseManager::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseManager::execute(const std::string& sql) {
    char* error = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &error) != SQLITE_OK) {
        std::cerr << "SQLite error: " << (error ? error : "unknown") << std::endl;
        sqlite3_free(error);
        return false;
    }
    return true;
}

bool DatabaseManager::beginTransaction() {
    return execute("BEGIN TRANSACTION;");
}

bool DatabaseManager::commitTransaction() {
    return execute("COMMIT;");
}

bool DatabaseManager::rollbackTransaction() {
    return execute("ROLLBACK;");
}

bool DatabaseManager::prepareStatement(const std::string& sql, sqlite3_stmt** stmt) {
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQLite prepare failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return true;
}

bool DatabaseManager::initializeSchema() {
    const char* sql = R"(
CREATE TABLE IF NOT EXISTS heroes (
    hero_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS hero_monsters (
    hero_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    health INTEGER NOT NULL,
    strength INTEGER NOT NULL,
    is_defeated INTEGER NOT NULL,
    slot INTEGER NOT NULL,
    PRIMARY KEY (hero_id, slot),
    FOREIGN KEY (hero_id) REFERENCES heroes(hero_id)
);
CREATE TABLE IF NOT EXISTS hero_inventory (
    hero_id INTEGER NOT NULL,
    item_name TEXT NOT NULL,
    description TEXT NOT NULL,
    status_effects TEXT NOT NULL,
    target_is_user INTEGER NOT NULL,
    effect_power INTEGER NOT NULL,
    slot INTEGER NOT NULL,
    PRIMARY KEY (hero_id, slot),
    FOREIGN KEY (hero_id) REFERENCES heroes(hero_id)
);
CREATE TABLE IF NOT EXISTS hero_stats (
    hero_id INTEGER NOT NULL,
    stat_name TEXT NOT NULL,
    stat_value INTEGER NOT NULL,
    PRIMARY KEY (hero_id, stat_name),
    FOREIGN KEY (hero_id) REFERENCES heroes(hero_id)
);
CREATE TABLE IF NOT EXISTS item_usage (
    hero_id INTEGER NOT NULL,
    item_name TEXT NOT NULL,
    usage_count INTEGER NOT NULL,
    PRIMARY KEY (hero_id, item_name),
    FOREIGN KEY (hero_id) REFERENCES heroes(hero_id)
);
CREATE TABLE IF NOT EXISTS monster_usage (
    hero_id INTEGER NOT NULL,
    monster_name TEXT NOT NULL,
    usage_count INTEGER NOT NULL,
    PRIMARY KEY (hero_id, monster_name),
    FOREIGN KEY (hero_id) REFERENCES heroes(hero_id)
);
CREATE TABLE IF NOT EXISTS global_stats (
    stat_name TEXT PRIMARY KEY NOT NULL,
    stat_value INTEGER NOT NULL
);
)";

    return execute(sql);
}

std::vector<HeroSummary> DatabaseManager::listHeroes() {
    std::vector<HeroSummary> heroes;
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement("SELECT hero_id, name FROM heroes;", &stmt)) {
        return heroes;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        HeroSummary summary;
        summary.id = sqlite3_column_int(stmt, 0);
        summary.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        heroes.push_back(summary);
    }

    sqlite3_finalize(stmt);
    return heroes;
}

bool DatabaseManager::loadHero(int heroId, Player*& player) {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement("SELECT name FROM heroes WHERE hero_id = ?;", &stmt)) {
        return false;
    }
    sqlite3_bind_int(stmt, 1, heroId);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return false;
    }

    std::string heroName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);

    player = new Player(heroName);

    // load monsters
    if (!prepareStatement("SELECT name, health, strength, is_defeated, slot FROM hero_monsters WHERE hero_id = ? ORDER BY slot;", &stmt)) {
        delete player;
        player = nullptr;
        return false;
    }
    sqlite3_bind_int(stmt, 1, heroId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int health = sqlite3_column_int(stmt, 1);
        int strength = sqlite3_column_int(stmt, 2);
        bool isDefeated = sqlite3_column_int(stmt, 3) != 0;
        Monster monster(name, health, strength);
        if (isDefeated) {
            monster.takeDamage(health);
        }
        player->addMonster(monster);
    }
    sqlite3_finalize(stmt);

    if (!prepareStatement("SELECT item_name, description, status_effects, target_is_user, effect_power, slot FROM hero_inventory WHERE hero_id = ? ORDER BY slot;", &stmt)) {
        delete player;
        player = nullptr;
        return false;
    }
    sqlite3_bind_int(stmt, 1, heroId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string statusEffects = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        bool targetIsUser = sqlite3_column_int(stmt, 3) != 0;
        int effectPower = sqlite3_column_int(stmt, 4);
        std::vector<std::string> effects = deserializeStatusEffects(statusEffects);

        Item item(name, description, effects, targetIsUser, effectPower);
        player->addItem(item);
    }
    sqlite3_finalize(stmt);

    return true;
}

bool DatabaseManager::saveHero(Player& player) {
    int heroId = -1;
    return saveHero(player, heroId);
}

bool DatabaseManager::saveHero(Player& player, int& heroId) {
    if (!beginTransaction()) {
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    if (heroId < 0) {
        if (!prepareStatement("INSERT INTO heroes (name) VALUES (?);", &stmt)) {
            rollbackTransaction();
            return false;
        }
        sqlite3_bind_text(stmt, 1, player.getPlayerName().c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            rollbackTransaction();
            return false;
        }
        heroId = static_cast<int>(sqlite3_last_insert_rowid(db));
        sqlite3_finalize(stmt);
    } else {
        if (!prepareStatement("UPDATE heroes SET name = ? WHERE hero_id = ?;", &stmt)) {
            rollbackTransaction();
            return false;
        }
        sqlite3_bind_text(stmt, 1, player.getPlayerName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, heroId);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            rollbackTransaction();
            return false;
        }
        sqlite3_finalize(stmt);

        const char* deleteStatements[] = {
            "DELETE FROM hero_monsters WHERE hero_id = ?;",
            "DELETE FROM hero_inventory WHERE hero_id = ?;"
        };

        for (const char* sql : deleteStatements) {
            if (!prepareStatement(sql, &stmt)) {
                rollbackTransaction();
                return false;
            }
            sqlite3_bind_int(stmt, 1, heroId);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                sqlite3_finalize(stmt);
                rollbackTransaction();
                return false;
            }
            sqlite3_finalize(stmt);
        }
    }

    if (!prepareStatement("INSERT INTO hero_monsters (hero_id, name, health, strength, is_defeated, slot) VALUES (?, ?, ?, ?, ?, ?);", &stmt)) {
        rollbackTransaction();
        return false;
    }

    auto& monsters = player.getMonsters();
    for (size_t i = 0; i < monsters.size(); ++i) {
        const Monster& monster = monsters[i];
        sqlite3_bind_int(stmt, 1, heroId);
        sqlite3_bind_text(stmt, 2, monster.getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, monster.getHealth());
        sqlite3_bind_int(stmt, 4, monster.getStrength());
        sqlite3_bind_int(stmt, 5, monster.getIsDefeated() ? 1 : 0);
        sqlite3_bind_int(stmt, 6, static_cast<int>(i));

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            rollbackTransaction();
            return false;
        }
        sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);

    if (!prepareStatement("INSERT INTO hero_inventory (hero_id, item_name, description, status_effects, target_is_user, effect_power, slot) VALUES (?, ?, ?, ?, ?, ?, ?);", &stmt)) {
        rollbackTransaction();
        return false;
    }

    auto& inventory = player.getInventory();
    for (size_t i = 0; i < inventory.size(); ++i) {
        const Item& item = inventory[i];
        sqlite3_bind_int(stmt, 1, heroId);
        sqlite3_bind_text(stmt, 2, item.getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, item.getDescription().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, serializeStatusEffects(item.getStatusEffects()).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 5, item.doesTargetUser() ? 1 : 0);
        sqlite3_bind_int(stmt, 6, item.getEffectPower());
        sqlite3_bind_int(stmt, 7, static_cast<int>(i));

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            rollbackTransaction();
            return false;
        }
        sqlite3_reset(stmt);
    }

    sqlite3_finalize(stmt);
    return commitTransaction();
}

std::string DatabaseManager::serializeStatusEffects(const std::vector<std::string>& effects) {
    std::ostringstream out;
    for (size_t i = 0; i < effects.size(); ++i) {
        out << effects[i];
        if (i + 1 < effects.size()) {
            out << ";";
        }
    }
    return out.str();
}

std::vector<std::string> DatabaseManager::deserializeStatusEffects(const std::string& value) {
    std::vector<std::string> effects;
    std::istringstream in(value);
    std::string item;
    while (std::getline(in, item, ';')) {
        if (!item.empty()) {
            effects.push_back(item);
        }
    }
    return effects;
}
