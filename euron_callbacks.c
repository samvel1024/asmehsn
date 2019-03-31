#include <stdint.h>
#include "euron_callbacks.h"
#include <assert.h>

uint64_t get_value(uint64_t n) {
  return n + 1;
}


void put_value(uint64_t n, uint64_t v) {
  assert(v == n + 4);
}