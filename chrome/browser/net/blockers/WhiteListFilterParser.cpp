#include "./WhiteListFilterParser.h"
#include <fstream>
#include <sstream>
#include "../../../../base/logging.h"

WhiteListFilterParser::WhiteListFilterParser() : src_data(nullptr) {
}

WhiteListFilterParser::~WhiteListFilterParser() {
    if (src_data) {
        delete[] src_data;
    }
}

bool WhiteListFilterParser::matches(const char *input, const char *contextDomain) {
    if(src_data != nullptr) {
        for(int i = 0; i < src_data_size; i++) {
            if(src_data[i] && (strstr(input, src_data[i]) != NULL || strstr(contextDomain, src_data[i]) != NULL)) {
                return true;
            }
        }
    }
    return false;
}


void WhiteListFilterParser::deserialize(char *buffer) {
    src_data_size = 1;
    const char *tmp = buffer;

    while((tmp = strstr(tmp, "|")) != NULL)
    {
        src_data_size++;
        tmp++;
    }
    src_data = new char*[src_data_size + 1];
    memset (src_data, 0, src_data_size + 1);

    int i = 0;

    char* pch = strtok (buffer, "|");
    while (pch != NULL)
    {
        src_data[i] = pch;
        pch = strtok (NULL, "|");
        i++;
    }
}
