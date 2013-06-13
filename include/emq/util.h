#ifndef EMQ_UTIL_H
#define EMQ_UTIL_H

/* zmq helpers */
#include "helpers.h"

void
emq_error (char *func);

void
emq_debug (char *message);

void
emq_info (char *message);

void
emq_enable_verbose ();

void
emq_disabled_verbose ();

void
emq_verbose (char *message);

#endif