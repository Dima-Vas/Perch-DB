
#pragma once

#include <string>

const std::string BASEPATH = std::string(std::getenv("PDB_DBPATH"));
const size_t MAXTABLEBUFFSIZE = std::stoull(std::string(std::getenv("PDB_TABLEBUFFSIZE")));

