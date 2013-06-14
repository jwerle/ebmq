/* std includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <zmq.h>

/* ebmq includes */
#include "ebmq/util.h"
#include "ebmq/server.h"

/**
 * Creates a new ebmq_server_t struct
 *
 * @param {char} *host
 * @return ebmq_server_t *
 */
ebmq_server_t *
ebmq_server_new (char *host) {
  // initialize a new ebmq_server_t
  ebmq_server_t *server = malloc(sizeof(ebmq_server_t));
  // set the host
  server->host = host; 
  // create context
  server->context = zmq_ctx_new();
  // assert context integrity
  assert(server->context);
  // create responder socket
  server->socket = zmq_socket(server->context, ZMQ_REP);
  // assert socket integrity
  assert(server->socket);
  // set unique ID on socket
  s_set_id(server->socket);
  // return to caller
  return server;
}

/**
 * Binds a new connection on a given ebmq_server_t struct
 *
 * @param {ebmq_server_t} *server
 */
void
ebmq_server_bind (ebmq_server_t *server) {
  int rc;
  // bind connection
  rc = zmq_bind(server->socket, server->host);
  if (rc != 0) ebmq_error("ebmq_server_bind");
}

/**
 * Listens to a bound connection
 *
 * @param {ebmq_server_t} *server
 */
void
ebmq_server_listen (ebmq_server_t *server, void (*callback) (char[])) {
  char *buffer;
  while (1) {
    buffer = s_recv(server->socket);
    if (buffer == NULL) ebmq_error("ebmq_server_listen");
    ebmq_server_reply_ok(server);
    callback(buffer);
    s_sleep(100);
  }
}

void
ebmq_server_reply_ok (ebmq_server_t *server) {
  int size;
  ebmq_debug("reply: ok");
  size = s_send(server->socket, "ok");
  if (size == -1) ebmq_error("ebmq_server_reply_ok");
}