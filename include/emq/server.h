#ifndef EMQ_SERVER_H
#define EMQ_SERVER_H

/**
 * EMQ server struct
 */

typedef struct {

  char *host;
  void *context;
  void *socket;

} emq_server_t;

/**
 * EMQ server prototypes
 */

emq_server_t *
emq_server_new (char *host);

void
emq_server_bind (emq_server_t *server);

void
emq_server_listen (emq_server_t *server, void (*callback) (char[]));

void
emq_server_reply_ok (emq_server_t *server);

void
emq_server_destroy (emq_server_t *server);

#endif