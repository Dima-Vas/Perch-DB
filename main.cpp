#include <iostream>
#include <fstream>
#include <string>
#ifdef __linux__
    #include <cstdlib>
#endif
#include "QueryParser.hpp"
#include "DataEngine.hpp"
#include "Table.hpp"

/*
Basic Functionality Implementation :
- Implement a basic data storage mechanism using file I/O operations.
- Design and implement data structures for storing tables, rows, and columns.
- Develop functionality for CRUD operations (Create, Read, Update, Delete) on data.
You can check .xml for some overall details.
*/
int main(int argc, char* argv[]) {
    std::cout << "\033[1;34mWelcome to Perch DB\033[0m\n" << std::endl;
    // auto& qp = QueryParser::getParserInstance();
    // std::string query = "CREATE DATABASE 'database'";
    // qp.processQuery(query);
    // const char* dbPath = std::getenv("PDB_DBPATH");
    // const char* metaPath = std::getenv("PDF_METAPATH");
    // if (!dbPath) {
    //     dbPath = "/opt/pdb/db/";
    // }
    // if (!metaPath) {
    //     metaPath = "/opt/pdb/meta/";
    // }
    // DataEngine de{dbPath, metaPath};
    // if (!de.deleteDatabase("database")) return 1;
    // de.createDatabase("database");

    size_t i = 0;
    while (i < 10000) {
        Table t(std::string("/opt/pdb/db/testdb/annual/"));
        t.readBuffers();
        RowBuffer* tt = t.getBuffer(0);
        PDataType** data_ptr = tt->getRow(99)->getRow(); // TODO : last row is not read so segfault and exceptions. All fine with user but not on large lad. Check commas in brackets
        size_t last_col = t.getStructure().size() - 1;
        switch (t.getStructure()[last_col]) {
            case PDataEnum::INT :
                std::cout << dynamic_cast<PInt*>(data_ptr[last_col])->val() << std::endl;
                break;
            case PDataEnum::STRING:
                std::cout << dynamic_cast<PString*>(data_ptr[last_col])->val() << std::endl;
                break;
        }
        ++i;
    }
    return 0;
}
