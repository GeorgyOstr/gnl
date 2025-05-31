#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char *str;
  while ((str = get_next_line(0))) {
    fputs(str, stdout);
    free(str);
  }
}
