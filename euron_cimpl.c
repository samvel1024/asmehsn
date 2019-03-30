#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "meet.h"
#include "euron.h"
#include "euron_callbacks.h"

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
  int64_t result;
} euron_t;

euron_t eurons[MAX_EURONS];

void eurassert(bool expr, eur_err err) {
  if (!expr) {
    fprintf(stderr, "Error: %d\n", err);
    exit(1);
  }
}

void push_stack(euron_t *eur, int64_t val) {
  eurassert(eur->stack_size < STACK_SIZE, stackoverflow);
  eur->stack[eur->stack_size++] = val;
}

int64_t pop_stack(euron_t *eur) {
  eurassert(eur->stack_size > 0, empty_stack_pop);
  return eur->stack[--eur->stack_size];
}

int64_t peek_stack(euron_t *eur) {
  eurassert(eur->stack_size > 0, empty_stack_pop);
  return eur->stack[-1 + eur->stack_size];
}


void print_stack(euron_t *e) {
  for (int i = 0; i < e->stack_size; ++i) {
    printf("%lld ", e->stack[i]);
  }
  printf("\n");
}

uint64_t euron(uint64_t euron_id, char *code) {
  euron_t *m = &eurons[euron_id];
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
        } else {
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
        uint64_t to = pop_stack(m);
        uint64_t msg = pop_stack(m);
        push_stack(m, exchange(euron_id, to, msg));
        break;
      }
      default: {
        eurassert(false, illegal_char);
      }
    }
    m->code_ptr += 1 + jmp;
  }
  m->result = peek_stack(m);
  return m->result;
}