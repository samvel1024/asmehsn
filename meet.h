#ifndef MEET
#define MEET

#include <stdint.h>

#define MAX_THREADS 1000


volatile int64_t mc[MAX_THREADS][MAX_THREADS]; // meet count
volatile int64_t mb[MAX_THREADS][MAX_THREADS]; // meet buffer


int64_t exchange(int64_t fr, int64_t to, int64_t msg) {

  while (mc[fr][to] > mc[to][fr]) {}

  mb[fr][to] = msg;
  mc[fr][to]++;

  while (mc[fr][to] > mc[to][fr]) {}

  int64_t ans = mb[to][fr];
  mc[fr][to]++;

  return ans;
}

#endif