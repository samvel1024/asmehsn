#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <assert.h>
#include "euron.h"
#include "euron_callbacks.h"

int64_t euron_count;

struct thread_args {
  uint64_t id;
  char *str;
  uint64_t  ans;
  pthread_t thr;
};

void thread_run(struct thread_args *args) {
  args->ans = euron(args->id, args->str);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage <euron_count> <input_string>");
    return 1;
  }

  assert(1 == sscanf(argv[1], "%lld", &euron_count));
  char *code = argv[2];
  struct thread_args args[euron_count];
  for (uint64_t i = 0; i < euron_count; ++i) {
    args[i].str = code;
    args[i].id = i;
    pthread_create(&args[i].thr, NULL, (void *(*)(void *)) thread_run, &args[i]);
  }

  printf("RESULTS\n");
  for (int i = 0; i < euron_count; ++i) {
    void *ptr;
    pthread_join(args[i].thr, &ptr);
    printf("%d \t %lld\n", i, args[i].ans);
  }

  return 0;
}
