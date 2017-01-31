#ifndef WHITELISTFILTERPARSER_H_
#define WHITELISTFILTERPARSER_H_

#include "./filter.h"
#include "BloomFilter.h"
#include "./cosmeticFilter.h"

class WhiteListFilterParser {
public:
  WhiteListFilterParser();
  ~WhiteListFilterParser();

  bool matches(const char *input, const char *contextDomain = nullptr);
  void deserialize(char *buffer);

  char *src_data;

};

#endif // ABPFILTERPARSER_H_
