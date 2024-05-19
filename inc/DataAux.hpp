#pragma once

#include <iostream>
#include <fstream>
#include <string>

// void parseCSV() {
//     std::string row;
//     size_t  rows_read = 0;
//     size_t  i = 0;
//     std::ostringstream entry;
//     bool in_quotes = false;
//     PDataType** toSet = new PDataType*[col_num];
//     DataRow* toPut;
//     size_t  curr_col = 0;

//     while (i < file_size && rows_read < MAXTABLEBUFFSIZE) {
//         char ch = file_data[i++];
//         if (in_quotes) {
//             if (ch == '"') {
//                 in_quotes = false;
//             } else {
//                 entry << ch;
//             }
//         } else {
//             if (ch == '"') {
//                 in_quotes = true;
//             } else if (ch == ',' || ch == '\n') {
//                 PDataEnum dataType = structure[curr_col];
//                 std::string x = entry.str();
//                 if (dataType == PDataEnum::PINTGR) {
//                     toSet[curr_col] = new PInt(stoi(entry.str()));
//                 } else if (dataType == PDataEnum::PSTRING) {
//                     toSet[curr_col] = new PString(entry.str());
//                 }
//                 entry.str("");
//                 entry.clear();
//                 x = entry.str();
//                 ++curr_col;
//             } else {
//                 entry << ch;
//             }
//         }
//         if (ch == '\n') {
//             toPut = new DataRow{col_num};
//             toPut->setData(toSet);
//             buffers[buff_idx].addRow(toPut);
//             ++i;
//             ++rows_read;
//             if (i < file_size && rows_read < MAXTABLEBUFFSIZE) {
//                 toSet = new PDataType*[col_num];
//             }
//             curr_col = 0;
//             continue;
//         }
//     }
// }
