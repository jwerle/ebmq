#ifndef EBMQ_UTIL_H
#define EBMQ_UTIL_H

/* zmq helpers */
#include "helpers.h"

void
ebmq_error (char *func);

void
ebmq_debug (char *message);

void
ebmq_info (char *message);

void
ebmq_enable_verbose ();

void
ebmq_disabled_verbose ();

void
ebmq_verbose (char *message);

#endif