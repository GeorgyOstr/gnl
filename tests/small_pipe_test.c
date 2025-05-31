#include "get_next_line.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  int fds[2];
  char *line;
  pipe(fds);

  dprintf(fds[1], "foo\nbarbaz");
  line = get_next_line(fds[0]);
  puts(line), free(line);
  line = get_next_line(fds[0]);
  dprintf(fds[1], "second\n");
  puts(line), free(line);
  
}
