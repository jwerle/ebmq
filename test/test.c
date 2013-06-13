/* std include */
#include <stdio.h>
#include <assert.h>

/* emq libs */
#include "emq/emq.h"

int
main (int argc, char *argv[]) {
  puts("\n[emq test]\n");
  /* assert version is defined */
  assert(EBMQ_VERSION); puts("  - version ok");
  return 0;
}