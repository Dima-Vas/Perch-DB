
#pragma once

#include <string>

#ifdef __linux__
const std::string BASEPATH = std::string(std::getenv("PDB_DBPATH"));
const size_t  MAXTABLEBUFFSIZE = std::stoull(std::string(std::getenv("PDB_TABLEBUFFSIZE")));
#endif

#ifdef _WIN32
const std::string BASEPATH = "C:\\Users\\PC\\Projects\\Perch-DB\\data\\";
const size_t  MAXTABLEBUFFSIZE = 500;
#endif