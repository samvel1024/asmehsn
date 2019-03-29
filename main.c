#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <assert.h>
#include "meet.h"

#define STACK_SIZE 1000000
#define MAX_EURONS 100

typedef enum eur_err {
  stackoverflow,
  empty_stack_pop,
  illegal_char,
  input_format_err,
  illegal_meet_thread
} eur_err;

typedef struct euron_t {
  int stack_size;
  char *code_ptr;
  int64_t stack[STACK_SIZE];
  pthread_t thread;
  int64_t result;
} euron;

euron eurons[MAX_EURONS];
uint64_t euron_count;

void eurassert(bool expr, eur_err err) {
  if (!expr) {
    fprintf(stderr, "Error: %d\n", err);
    exit(1);
  }
}


void push_stack(euron *eur, int64_t val) {
  eurassert(eur->stack_size < STACK_SIZE, stackoverflow);
  eur->stack[eur->stack_size++] = val;
}

int64_t pop_stack(euron *eur) {
  eurassert(eur->stack_size > 0, empty_stack_pop);
  return eur->stack[--eur->stack_size];
}

int64_t peek_stack(euron *eur) {
  eurassert(eur->stack_size > 0, empty_stack_pop);
  return eur->stack[-1 + eur->stack_size];
}

uint64_t get_value(uint64_t n) {
  assert(n < euron_count);
  return n + 1;
}

void put_value(uint64_t n, uint64_t v) {
  assert(n < euron_count);
  assert(v == n + 4);
}

void print_stack(euron *e){
  for(int i=0; i<e->stack_size; ++i){
    printf("%lld ", e->stack[i]);
  }
  printf("\n");
}

void euron_loop(uint64_t euron_id, char *code) {
  euron *m = &eurons[euron_id];
  m->stack_size = 0;
  m->code_ptr = code;
  char curr;
  while ((curr = *m->code_ptr) != '\0') {
    int64_t jmp = 0;
    switch (curr) {
      case '+': {
        push_stack(m, pop_stack(m) + pop_stack(m));
        break;
      }
      case '*': {
        push_stack(m, pop_stack(m) * pop_stack(m));
        break;
      }
      case '-': {
        push_stack(m, -pop_stack(m));
        break;
      }
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': {
        int val = curr - '0';
        push_stack(m, val);
        break;
      }
      case 'n': {
        push_stack(m, euron_id);
        break;
      }
      case 'B': {
        int64_t j = pop_stack(m);
        if (peek_stack(m) != 0) {
          jmp = j;
        }else {
        }
        break;
      }
      case 'C': {
        pop_stack(m);
        break;
      }
      case 'D': {
        push_stack(m, peek_stack(m));
        break;
      }
      case 'E': {
        int64_t f = pop_stack(m);
        int64_t ff = pop_stack(m);
        push_stack(m, f);
        push_stack(m, ff);
        break;
      }
      case 'G': {
        push_stack(m, get_value(euron_id));
        break;
      }
      case 'P': {
        put_value(euron_id, (uint64_t) pop_stack(m));
        break;
      }
      case 'S': {
        push_stack(m, exchange(euron_id, pop_stack(m), pop_stack(m)));
        break;
      }
      default: {
        eurassert(false, illegal_char);
      }
    }
    m->code_ptr += 1 + jmp;
  }
  m->result = peek_stack(m);
}

struct thread_args {
  uint64_t id;
  char *str;
};

void thread_run(struct thread_args *args) {
  euron_loop(args->id, args->str);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage <euron_count> <input_string>");
    return 1;
  }

  eurassert(1 == sscanf(argv[1], "%lld", &euron_count), input_format_err);
  char *code = argv[2];

  struct thread_args args[euron_count];
  for (uint64_t i = 0; i < euron_count; ++i) {
    args[i].str = code;
    args[i].id = i;
    pthread_create(&eurons[i].thread, NULL, (void *(*)(void *)) thread_run, &args[i]);
  }

  printf("RESULTS\n");
  for (int i = 0; i < euron_count; ++i) {
    void *ptr;
    pthread_join(eurons[i].thread, &ptr);
    printf("%d \t %lld\n", i, eurons[i].result);
  }

  return 0;
}
