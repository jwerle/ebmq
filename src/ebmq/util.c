/* std includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <logger.h>
#include <zmq.h>

bool ebmq_verbose_enabled = false;

void
ebmq_log (char *message) {
  puts(message);
}

void
ebmq_error (char *func) {
  puts(func);
  exit(EXIT_FAILURE);
}

void
ebmq_debug (char *message) {
  logger_debug(message);
}

void
ebmq_info (char *message) {
  logger_info(message);
}

void
ebmq_enable_verbose () {
  ebmq_verbose_enabled = (bool) true;
}

void
ebmq_disabled_verbose () {
  ebmq_verbose_enabled = (bool) false;
}

void
ebmq_verbose (char *message) {
  if (ebmq_verbose_enabled == false) return;
  printf("  verbose: ");
  puts(message);
}