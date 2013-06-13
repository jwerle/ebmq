/* std includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <zmq.h>

/* emq includes */
#include "emq/util.h"
#include "emq/server.h"

/**
 * Creates a new emq_server_t struct
 *
 * @param {char} *host
 * @return emq_server_t *
 */
emq_server_t *
emq_server_new (char *host) {
  // initialize a new emq_server_t
  emq_server_t *server = malloc(sizeof(emq_server_t));
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
 * Binds a new connection on a given emq_server_t struct
 *
 * @param {emq_server_t} *server
 */
void
emq_server_bind (emq_server_t *server) {
  int rc;
  // bind connection
  rc = zmq_bind(server->socket, server->host);
  if (rc != 0) emq_error("emq_server_bind");
}

/**
 * Listens to a bound connection
 *
 * @param {emq_server_t} *server
 */
void
emq_server_listen (emq_server_t *server, void (*callback) (char[])) {
  char *buffer;
  while (1) {
    emq_debug("waiting");
    buffer = s_recv(server->socket);
    if (buffer == NULL) emq_error("emq_server_listen");
    emq_debug("Got message");
    emq_server_reply_ok(server);
    callback(buffer);
    s_sleep(100);
  }
}

void
emq_server_reply_ok (emq_server_t *server) {
  int size;
  emq_debug("reply: ok");
  size = s_send(server->socket, "ok");
  if (size == -1) emq_error("emq_server_reply_ok");
}