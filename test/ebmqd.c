/* std include */
#include <stdio.h>
#include <assert.h>

/* ebmq libs */
#include "ebmq/ebmq.h"
#include "ebmq/util.h"
#include "ebmq/client.h"


/**
 * client messages
 */
char *messages[] = {
  "hi",
  "hey",
  "hello",
  "yo",
  "aye",
  "suppp",
  "whatttuppp"
};

/**
 * number of incoming server request
 */

int n_requests = 0;

/**
 * number of message to send
 */
int n_messages = sizeof(messages) / sizeof(char *);


int
main (int argc, char *argv[]) {
  puts("\n[ebmqd test]\n");

  char str[256];
  ebmq_client_t *client = ebmq_client_new("tcp://localhost:5555");
  ebmq_client_connect(client);
  
  for (int i = 0; i < n_messages; ++i) {
    sprintf(str, "messaging host server: (%d), '%s'", i, messages[i]);
    ebmq_debug(str);
    ebmq_client_message(client, messages[i]);
  }

  ebmq_client_disconnect(client);

  return 0;
}