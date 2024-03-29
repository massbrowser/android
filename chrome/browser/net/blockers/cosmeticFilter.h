#ifndef COSMETICFILTER_H_
#define COSMETICFILTER_H_

#include <math.h>
#include <string.h>
#include "HashSet.h"

class CosmeticFilter {
 public:
  uint64_t hash() const;

  ~CosmeticFilter() {
    if (data) {
      delete[] data;
    }
  }
  explicit CosmeticFilter(const char *data) {
    size_t len = strlen(data) + 1;
    this->data = new char[len];
    snprintf(this->data, len, "%s", data);
  }

  CosmeticFilter(const CosmeticFilter &rhs) {
    data = new char[strlen(rhs.data) + 1];
    memcpy(data, rhs.data, strlen(rhs.data) + 1);
  }

  CosmeticFilter() : data(nullptr) {
  }

  bool operator==(const CosmeticFilter &rhs) const {
    return !strcmp(data, rhs.data);
  }

  bool operator!=(const CosmeticFilter &rhs) const {
    return !(*this == rhs);
  }

  // Nothing needs to be updated for multiple adds
  void update(const CosmeticFilter &) {}

  uint32_t serialize(char *buffer) {
    if (buffer) {
      memcpy(buffer, data, strlen(data) + 1);
    }
    return static_cast<uint32_t>(strlen(data)) + 1;
  }

  uint32_t deserialize(char *buffer, uint32_t bufferSize) {
    int len = static_cast<int>(strlen(buffer));
    data = new char[len + 1];
    memcpy(data, buffer, len + 1);
    return len + 1;
  }

  char *data;
};

class CosmeticFilterHashSet : public HashSet<CosmeticFilter> {
 public:
  CosmeticFilterHashSet() : HashSet<CosmeticFilter>(1000) {
  }
  char * toStylesheet(uint32_t *len) {
    *len = fillStylesheetBuffer(nullptr);
    char *buffer = new char[*len];
    memset(buffer, 0, *len);
    fillStylesheetBuffer(buffer);
    return buffer;
  }

 private:
  uint32_t fillStylesheetBuffer(char *buffer) {
    uint32_t len = 0;
    for (uint32_t bucketIndex = 0; bucketIndex < bucketCount; bucketIndex++) {
      HashItem<CosmeticFilter> *hashItem = buckets[bucketIndex];
      len = 0;
      while (hashItem) {
        CosmeticFilter *cosmeticFilter = hashItem->hashItemStorage;
        // [cosmeticFilter],[space]
        int cosmeticFilterLen =
          static_cast<int>(strlen(cosmeticFilter->data));
        if (buffer) {
          memcpy(buffer + len, cosmeticFilter->data, cosmeticFilterLen);
        }
        len += cosmeticFilterLen;
        if (hashItem->next) {
          if (buffer) {
            memcpy(buffer + len, ", ", 2);
          }
          len += 2;
        }
        hashItem = hashItem->next;
      }
    }
    return len;
  }
};

#endif // COSMETICFILTER_H_