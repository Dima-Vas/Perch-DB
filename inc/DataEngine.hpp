
#pragma once

#include <string>
#include "Table.hpp"

class DataEngine {
private:
    const char* dbBasePath;
    const char* metaBasePath;

    std::string dataEnumToString(PDataEnum dataEnum) const;
public:

    constexpr DataEngine() noexcept {};

    ~DataEngine() = default;

    void initTable(const std::string& tableName, const std::vector<std::string>& columnNames,
                int rowsReserve, int primaryKeyIndex, const std::vector<PDataEnum>& columnTypes);

    Table getTable(const std::string& tableName);
};

