
#include <optional>
#include "QueryParser.hpp"


void QueryParser::processQuery(const std::string& query) {
    std::stringstream currWordBuffer;
    for (char curr : query) {
        if (curr == '\'') {
            if (isReadingVal) {
                std::cout << "Read the value :" << currWordBuffer.str() << std::endl;
                currWordBuffer.str("");
            }
            isReadingVal ^= 1;
            continue;
        }
        if (curr == ' ' && !isReadingVal) {
            continue;
        }
        currWordBuffer << curr;

        if (isReadingVal)
            continue;
        std::optional<QueryToken> token = checkIfToken(currWordBuffer);
        if (token.has_value()) {
            currWordBuffer.str("");
        }
    }
}
