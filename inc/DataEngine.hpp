
#pragma once

#include <string>
#include "Table.hpp"

class DataEngine {
private:
    const char* dbBasePath;
    const char* metaBasePath;
public:

    constexpr DataEngine() noexcept;

    constexpr DataEngine(const char* aDbBasePath, const char* aMetaBasePath) noexcept : 
    dbBasePath(aDbBasePath),
    metaBasePath(aMetaBasePath) {};

    ~DataEngine() = default;

    bool createDatabase(const std::string& name);

    bool deleteDatabase(const std::string& name);

    bool createTable(const std::string& tableName, const std::string& dbName);

    Table getTable(const std::string& tableName, const std::string& dbName);
};

