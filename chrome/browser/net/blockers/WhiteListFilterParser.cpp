#include "./WhiteListFilterParser.h"
#include "../../../../base/android/apk_assets.h"

WhiteListFilterParser::WhiteListFilterParser() : src_data(nullptr) {
}

WhiteListFilterParser::~WhiteListFilterParser() {
    if (src_data) {
        delete[] src_data;
    }
}

bool WhiteListFilterParser::matches(const char *input, const char *contextDomain) {
    if(this->src_data != nullptr) {
        char* pch = strtok (src_data,"|");
        while (pch != NULL)
        {
            if(strstr(input, pch) != NULL) {
                return true;
            }
            pch = strtok (NULL, "|");
        }
    }
    return false;
}


void WhiteListFilterParser::deserialize(char *buffer) {
  int len = strlen(buffer);
  src_data = new char[len];
  memcpy(src_data, buffer, len);
}
