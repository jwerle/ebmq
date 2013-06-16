#ifndef EBMQ_CLIENT_H
#define EBMQ_CLIENT_H

/*
 * default buffer size.
 */

#ifndef EBMQ_CLIENT_DEFAULT_RECV_TIMEOUT
#define EBMQ_CLIENT_DEFAULT_RECV_TIMEOUT 1000
#endif


/**
 * embq client struct
 */

typedef struct {
  
  char *host;
  void *context;
  void *socket;

} ebmq_client_t;


/**
 * prototypes
 */

ebmq_client_t *
ebmq_client_new (char *host);

void
ebmq_client_connect (ebmq_client_t *client);

void
ebmq_client_message (ebmq_client_t *client, char *message);

void
ebmq_client_disconnect (ebmq_client_t *client);

#endif