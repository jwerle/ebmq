/* std includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <logger.h>
#include <zmq.h>

bool emq_verbose_enabled = false;

void
emq_error (char *func) {
  printf("error: ");
  puts(func);
  exit(EXIT_FAILURE);
}

void
emq_debug (char *message) {
  logger_debug(message);
}

void
emq_info (char *message) {
  logger_info(message);
}

void
emq_enable_verbose () {
  emq_verbose_enabled = (bool) true;
}

void
emq_disabled_verbose () {
  emq_verbose_enabled = (bool) false;
}

void
emq_verbose (char *message) {
  if (emq_verbose_enabled == false) return;
  printf("  verbose: ");
  puts(message);
}