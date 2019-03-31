#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

char cmds[] = {
    '+', '*',
    '-',
    '0','1', '2', '3', '4', '5', '6', '7', '8', '9',
    'n', 'B', 'C', 'D', 'E', 'G', 'P'
};


int main(int argc, char **argv) {
  int size = 0;
  if (argc != 3) {
    printf("Usage testgen <seed> <length>");
    return 1;
  }
  int seed;
  sscanf(argv[1], "%d", &seed);
  srand(seed);
  sscanf(argv[2], "%d", &size);
  for (int i = 0; i < size; ++i) {
    int index = random() % sizeof(cmds);
    putchar(cmds[index]);
  }
  putchar('\n');
}

