#include <stdint.h>
#define MAX_THREADS 1000

volatile int32_t meet[MAX_THREADS][MAX_THREADS]; // meet count
volatile int64_t buff[MAX_THREADS]; // meet buffer


int64_t thread_meet(int64_t fr, int64_t to, int64_t msg) {

  while (meet[fr][to] > meet[to][fr]) {}

  buff[fr] = msg;
  meet[fr][to]++;

  while (meet[fr][to] > meet[to][fr]) {}

  int64_t ans = buff[to];
  meet[fr][to]++;

  return ans;
}