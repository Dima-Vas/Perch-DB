#include <iostream>
#include <fstream>
#include "QueryParser.hpp"
#include <string>

/*
Basic Functionality Implementation :
- Implement a basic data storage mechanism using file I/O operations.
- Design and implement data structures for storing tables, rows, and columns.
- Develop functionality for CRUD operations (Create, Read, Update, Delete) on data.
You can check .xml for some overall details.
*/
int main(int argc, char* argv[]) {
    std::cout << "\033[1;34mWelcome to Perch DB\033[0m\n" << std::endl;
    auto& qp = QueryParser::getParserInstance();
    std::string query = "CREATE DATABASE 'database'";
    qp.processQuery(query);

    return 0;
}
