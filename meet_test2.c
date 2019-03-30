#include "meet.h"
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

int THREAD_COUNT;

struct thread {
  int id;
  pthread_t thr;
  int64_t before;
  int64_t ans;
};

void thread_run(struct thread *args) {
  int id = args->id;
  args->before = 0;
  if (id > 0 && id < THREAD_COUNT - 1) {
    args->before = exchange(id, id - 1, 0) + args->id;
    exchange(id, id + 1, args->before);
    args->ans = exchange(id, id + 1, 0);
    exchange(id, id-1, args->ans);
  } else if (id == 0){
    exchange(id, id + 1, args->before);
    args->ans = exchange(id, id+1, 0);
  } else if (id == THREAD_COUNT - 1){
    args->before = args->id + exchange(id, id-1, 0);
    args->ans = args->before;
    exchange(id, id-1, args->before);
  }
}

int main(int argc, char **argv) {

  if (argc != 2) return 1;

  sscanf(argv[1], "%d", &THREAD_COUNT);

  struct thread th[THREAD_COUNT];
  for (int i = 0; i < THREAD_COUNT; ++i) {
    th[i].id = i;
    pthread_create(&th[i].thr, NULL, (void *(*)(void *)) thread_run, &th[i].id);
  }

  void *ptr;
  for (int i = 0; i < THREAD_COUNT; ++i) {
    pthread_join(th[i].thr, &ptr);
    long before = (i*i + i)/2;
    long ans = (THREAD_COUNT * THREAD_COUNT + THREAD_COUNT)/2 - THREAD_COUNT;
    assert(before == th[i].before && ans == th[i].ans);
    printf("Thread %d: before=%lld, ans=%lld\n", i, th[i].before, th[i].ans);
  }
}