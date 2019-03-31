#include "meet.h"
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <jmorecfg.h>
#include <string.h>
int THREAD_COUNT;
int PING_COUNT;

struct thread {
  int id;
  pthread_t thr;
  int64_t before;
  int64_t ans;
};

void thread_run(struct thread *args) {
  int id = args->id;
  int ctr[THREAD_COUNT];
  for (int receiver = 0; receiver < THREAD_COUNT; ++receiver) {
    memset(ctr, 0, sizeof(int) * THREAD_COUNT);
    for (int __j = 0; __j < PING_COUNT; ++__j) {
      if (receiver == id) {
        for (int th = 0; th < THREAD_COUNT; ++th) {
          int val = thread_meet(id, th, id);
          assert(val == th);
          ctr[val]++;
        }
      } else {
        int rcv = thread_meet(id, receiver, id);
        assert(rcv == receiver);
      }
    }
    if (receiver == id) {
      for (int i = 0; i < THREAD_COUNT; ++i)
        assert(ctr[i] == PING_COUNT);
    }
  }
}

int main(int argc, char **argv) {

  if (argc != 3) return 1;

  sscanf(argv[1], "%d", &THREAD_COUNT);
  sscanf(argv[2], "%d", &PING_COUNT);

  struct thread th[THREAD_COUNT];
  for (int i = 0; i < THREAD_COUNT; ++i) {
    th[i].id = i;
    pthread_create(&th[i].thr, NULL, (void *(*)(void *)) thread_run, &th[i].id);
  }

  void *ptr;
  for (int i = 0; i < THREAD_COUNT; ++i) {
    pthread_join(th[i].thr, &ptr);
  }
}