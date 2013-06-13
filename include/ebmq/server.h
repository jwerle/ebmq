#ifndef EBMQ_SERVER_H
#define EBMQ_SERVER_H

/**
 * ebmq server struct
 */

typedef struct {

  char *host;
  void *context;
  void *socket;

} ebmq_server_t;

/**
 * ebmq server prototypes
 */

ebmq_server_t *
ebmq_server_new (char *host);

void
ebmq_server_bind (ebmq_server_t *server);

void
ebmq_server_listen (ebmq_server_t *server, void (*callback) (char[]));

void
ebmq_server_reply_ok (ebmq_server_t *server);

void
ebmq_server_destroy (ebmq_server_t *server);

#endif