/* std includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <zmq.h>

/* ebmq includes */
#include "ebmq/util.h"
#include "ebmq/client.h"

/**
 * Creates a new ebmq_client_t struct
 *
 * @param {char} *host
 * @return ebmq_client_t *
 */
ebmq_client_t *
ebmq_client_new (char *host) {
  // initialize a new ebmq_client_t
  ebmq_client_t *client = malloc(sizeof(ebmq_client_t));
  // set the host
  client->host = host; 
  // create context
  client->context = zmq_ctx_new();
  // assert context integrity
  assert(client->context);
  // create responder socket
  client->socket = zmq_socket(client->context, ZMQ_REQ);
  // assert socket integrity
  assert(client->socket);
  // set unique ID on socket
  s_set_id(client->socket);
  // return to caller
  return client;
}

void
ebmq_client_connect (ebmq_client_t *client) {
  int rc, timeout;

  rc = zmq_connect (client->socket, client->host);
  timeout = EBMQ_CLIENT_DEFAULT_RECV_TIMEOUT;

  if (rc != 0) ebmq_error("ebmq_client_connect");

  zmq_setsockopt(client->socket, ZMQ_RCVTIMEO, (const void *)timeout, 0);
}

void
ebmq_client_message (ebmq_client_t *client, char *message) {
  int size;
  char *buffer;

  size = s_send (client->socket, message);
  buffer = s_recv(client->socket);

  if (size != strlen(message)) ebmq_error("ebmq_client_message");
  else if (buffer == NULL) ebmq_error("ebmq_client_message");
}

void
ebmq_client_disconnect (ebmq_client_t *client) {
  zmq_close (client->socket);
  zmq_ctx_destroy (client->context);
}