
#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <optional>
#include <string.h>
#include <errno.h>
#include "DataAux.hpp"
#include "DataEngine.hpp"
#include "Table.hpp"
#include "defs.h"

std::string DataEngine::dataEnumToString(PDataEnum dataEnum) const {
    switch (dataEnum) {
        case PINTGR: return "Int";
        case PSTRING: return "Str";
        case PDOUBLE: return "Dbl";
        default: throw std::invalid_argument("Bad dataEnumToString() : unknown PDataEnum");
    }
}

/**
 * @brief creates table's metadata and locates file with table's content in .csv format
 * 
 * @param tableName name of the table
 * @param dbName name of the DB to put the table into
 * @return boolean indicating success of an operation.
 */
void DataEngine::initTable(const std::string& tableName, const std::vector<std::string>& columnNames,
                int rowsReserve, int primaryKeyIndex, const std::vector<PDataEnum>& columnTypes) {
    size_t column_num = columnNames.size();
    if (column_num != columnTypes.size()) {
        std::cerr << "Bad createTable() : coulmn names size is " << column_num << " and column types size is " 
            << columnTypes.size() << std::endl;
        throw std::invalid_argument("Bad createTable : column names and types size mismatch");
    }
#ifdef __linux__
    std::string table_path = BASEPATH + "/" + tableName + "/";
#endif
#ifdef _WIN32
    std::string table_path = BASEPATH + tableName + "\\";
#endif
    if (!std::filesystem::create_directory(table_path)) {
        std::cerr << "Bad createTable() : directory with path " << table_path << " could not be created : " << strerror(errno) << std::endl;
    };
#ifdef __linux__
    std::ofstream metaFile(table_path + "/meta.pdb");
#endif
#ifdef _WIN32
    std::ofstream metaFile(table_path + "\\meta.pdb");
#endif
    
    if (!metaFile.is_open()) {
        std::cerr << "Bad createTable() : could not create meta file : " << strerror(errno) << std::endl;
        throw std::runtime_error("Bad createTable() : could not create meta file");
    }
    metaFile << tableName << "\n";

    for (size_t i = 0; i < column_num; ++i) {
        metaFile << dataEnumToString(columnTypes[i]) << " ";
    }
    metaFile << "\n";
    for (size_t i = 0; i < column_num; ++i) {
        std::string column_name = columnNames[i];
        std::replace(column_name.begin(), column_name.end(), ' ', '_');
        metaFile << column_name << " ";
    }
    metaFile << "\n";
    metaFile << columnNames[primaryKeyIndex] << "\n";

    int numFiles = (rowsReserve + MAXTABLEBUFFSIZE - 1) / MAXTABLEBUFFSIZE;
    metaFile << numFiles;
    metaFile.close();

    for (int i = 1; i <= numFiles; ++i) {
        std::ofstream dataFile(table_path + std::to_string(i) + ".pdb");
        if (!dataFile.is_open()) {
            std::cerr << "Bad createDatabase() : could not create data file " + std::to_string(i) << std::endl;
            throw std::runtime_error("Bad createDatabase() : could not create data file");
        }
        dataFile.close();
    }
}

Table DataEngine::getTable(const std::string& tableName) {
    std::ostringstream ss;
#ifdef __linux__
    ss << BASEPATH << "/" << tableName << "/";
#endif
#ifdef _WIN32
    ss << BASEPATH << "\\" << tableName << "\\";
#endif
    Table output{ss.str()};
    return output;
}
