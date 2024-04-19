#pragma once

#include <iostream>
#include <fstream>
#include <string>

std::string dbCreateError = "Bad DB creation ";
std::string dbDeleteError = "Bad DB deletion ";


bool createAndWriteMeta(const std::string& metaPath, const std::string& metadata) {
    std::ofstream file(metaPath);
    if (!file.is_open()) {
        return false;
    }
    file << metadata;
    file.close();
    return true;
}

constexpr const std::string& graspMeta(const std::string& entityName) {
    return const_cast<std::string&>(entityName);
}
