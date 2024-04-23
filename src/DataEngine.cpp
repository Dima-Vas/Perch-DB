
#include <iostream>
#include <string>
#include <filesystem>
#ifdef __linux
    #include <unistd.h>
    #include <sys/stat.h>
#endif
#include <fstream>
#include <optional>
#include <string.h>
#include <errno.h>
#include "DataAux.hpp"
#include "DataEngine.hpp"
#include "Table.hpp"

/**
 * @brief Creates new DB and metadata for it. 
 * If fails, cleans all the files created by itself.
 * 
 * @param name name of the DB to create.
 * @return boolean indicating if new DB was indeed created.
 */
bool DataEngine::createDatabase(const std::string& name) {
    std::filesystem::path dbPath = dbBasePath + name;
    if (std::filesystem::exists(dbPath)) {
        std::cerr << dbCreateError << ": already exists" << std::endl;
        return false;
    }
    try {
        std::filesystem::create_directory(dbPath);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << dbCreateError << ": " << e.what() << std::endl;
        return false;
    }
    if (chmod(dbPath.c_str(), S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0) {
        std::cerr << dbCreateError << "when setting access: " << strerror(errno) << std::endl;
        std::filesystem::remove(dbPath);
        return false;
    }
    if (!createAndWriteMeta(std::string{dbPath} + "/meta.pdb", graspMeta(name))) {
        std::cerr << dbCreateError << "when creating metadata: " << strerror(errno) << std::endl;
        std::filesystem::remove_all(dbPath);
        return false;
    }
    return true;
}

/**
 * @brief Attempts to delete a DB and to erase all its content and metadata.
 * 
 * @param name name of the DB to delete.
 * @return true if DB with such name does not exist, before or after execution; false if fails to delete existing DB.
 */
bool DataEngine::deleteDatabase(const std::string& name) {
    std::filesystem::path dbPath = dbBasePath + name;
    try {
        if (!std::filesystem::exists(dbPath) || !std::filesystem::is_directory(dbPath)) {
            return true;
        }
        std::filesystem::remove_all(dbPath);
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << dbDeleteError << ": " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief creates table's metadata and locates file with table's content in .csv format
 * 
 * @param tableName name of the table
 * @param dbName name of the DB to put the table into
 * @return boolean indicating success of an operation.
 */
bool DataEngine::createTable(const std::string& tableName, const std::string& dbName) {
    std::ofstream { "Hello.txt" };
    return false;
}

Table DataEngine::getTable(const std::string& tableName, const std::string& dbName) {
    std::ostringstream ss;
    ss << this->dbBasePath << "/" << dbName << "/" << tableName << "/";
    Table output{ss.str()};
    return output;
}
