#include "meet.h"
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

int MEET_COUNT;
int THREAD_COUNT;

struct thread {
  int id;
  pthread_t thr;
};

void thread_run(struct thread *args) {
  int mid = args->id;
  int to = mid % 2 == 0 ? mid + 1 : mid - 1;
  for(int i=0; i<MEET_COUNT; ++i) {
    int64_t ex = thread_meet(mid, to, i);
    if (ex != i) {
      fprintf(stderr, "ERROR id=%d, got=%lld, exp=%d\n",mid, ex, i);
      exit(1);
    }
  }
}

int main(int argc, char **argv){

  if (argc != 3) return 1;

  sscanf(argv[1], "%d", &THREAD_COUNT);
  sscanf(argv[2], "%d", &MEET_COUNT);

  struct thread th[THREAD_COUNT];
  for(int i=0; i<THREAD_COUNT; ++i){
    th[i].id = i;
    pthread_create(&th[i].thr, NULL, (void *(*)(void *)) thread_run, &th[i].id);
  }

  void *ptr;
  for(int i=0; i<THREAD_COUNT; ++i)
    pthread_join(th[i].thr, &ptr);
}