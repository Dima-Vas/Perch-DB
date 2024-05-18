

#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <ctime>
#include <cstdlib>

// Identifier for the records
// Generate an identifier straight after instantiation
// Records are transactions
// Need strong hashing system
class RecordID {
public:
    RecordID() {
        generateID();
    };

    ~RecordID();

    size_t  val() {
        return id;
    }

private:
    size_t  id;

    void generateID() {
        id = std::hash<std::string>{}("ID_" + std::to_string(std::time(nullptr)) + "_" + std::to_string(rand() % 100));
    }
};


