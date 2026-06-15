#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>
#include "Player.h"

struct HeroSummary {
    int id;
    std::string name;
};

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    bool open(const std::string& path);
    void close();

    std::vector<HeroSummary> listHeroes();
    bool loadHero(int heroId, Player*& player);
    bool saveHero(Player& player);
    bool saveHero(Player& player, int& heroId);

private:
    sqlite3* db;

    bool execute(const std::string& sql);
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    bool prepareStatement(const std::string& sql, sqlite3_stmt** stmt);

    bool initializeSchema();
    std::string serializeStatusEffects(const std::vector<std::string>& effects);
    std::vector<std::string> deserializeStatusEffects(const std::string& value);
};

#endif