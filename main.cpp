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
    size_t i = 0;
    size_t cycles = std::stoull(argv[1]);
    while (i < cycles) {
        Table t{"/opt/pdb/db/testdb/annual/"};
        PDataType** data_ptr = t.get(612)->getData();
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
