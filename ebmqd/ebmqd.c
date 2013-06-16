/* std includes */
#include <stdlib.h>
#include <assert.h>

/* ebmq includes */
#include "ebmq/util.h"
#include "ebmqd.h"
#include "commander.h"


/**
 * Command options
 */

static void
verbose_opt (command_t *self) {
  ebmq_enable_verbose();
}


/**
 * main function 
 */

int
main (int argc, char *argv[]) {
  command_t program;

  if (argc > 1 && strncmp(&argv[1][0], "-", 1)) {
    /**
     * we need to figure out the
     * action being taken on the
     * daemon process
     */
    if (ebmqd_has_command(argv, "start")) {
      if (ebmqd_is_alive()) {
        ebmq_error("ebmqd: daemon is already running");
      } else {
        ebmqd_start();
      }
    } else if (ebmqd_has_command(argv, "stop")) {
      if (ebmqd_is_alive()) {
        ebmqd_stop();
      } else {
        ebmq_error("ebmqd: (nod pid file) not running");
      }
    }
    else if (ebmqd_has_command(argv, "restart")) {
      ebmqd_restart();
    } else {
      char *str;
      sprintf(str,"ebmqd: invalid command `%s`", argv[1]);
      ebmq_error(str);
    }
  }

  // initialize program setting program version
  command_init(&program, "ebmqd [start|stop|restart]", EBMQ_VERSION);

  command_option(&program, "-v", "--verbose", "enable verbose logging", verbose_opt);

  // parse commands
  command_parse(&program, argc, argv);

  // free program
  command_free(&program);
  return 0;
}


/**
 * Starts the daemon
 */
void
ebmqd_start () {
  pid_t pid, sid;
  char *buffer;

  if (ebmqd_is_root() == 0) {
    ebmq_error("Operation not permitted");
  }

  // /** 
  //  * Flush stdout, stdin, and stderr
  //  * so forked process does not inherit
  //  * streams
  //  */
  // fflush(stdout);
  // fflush(stdin);
  // fflush(stderr);

  // /**
  //  * set the pid and exit with failure if it
  //  * failed to return one greater than `0`
  //  * else exit with success
  //  */
  // pid = fork();
  // if (pid < 0) {
  //   ebmqd_log(LOG_ERR, "failed to fork child");
  //   ebmq_error("ebmqd: failed to fork and get `pid`");
  // }

  // if (pid > 0) {
  //   // write pid to file
  //   ebmqd_write_pid(pid);
  //   ebmqd_set_opt(EBMQD_CHILD, 1);
  //   exit(EXIT_SUCCESS);
  // }

  // ebmqd_set_pid(pid);

  // // ensure the pid was set internally
  // assert(ebmqd_get_pid() > 0);

  // // set the unmask to `0`
  // umask(0);

  // /**
  //  * set the sid and exit with failure if
  //  * it failed to return one greater than `0`
  //  */
  // sid = setsid();
  // if (sid < 0) {
  //   ebmqd_log(LOG_ERR, "failed to obtain sid");
  //   exit(EXIT_FAILURE);
  // }

  // ebmqd_set_sid(sid);

  // // ensure the sid was set internally
  // assert(ebmqd_get_sid() > 0);

  // /**
  //  * set root working directory
  //  * and exit with a failure if
  //  * it failed to return greater
  //  * than `0`
  //  */
  // if (chdir("/") < 0) {
  //   exit(EXIT_FAILURE);
  // }

  ebmq_log("ebmqd: starting daemon");
  ebmqd_log(LOG_INFO, "starting");

  // close all file descriptors
  //ebmqd_close_fds();

  // open logs
  ebmqd_open_logs();

  // create a new server struct
  ebmq_server_t *server = ebmq_server_new("tcp://*:5555");

  // bind connection
  ebmq_server_bind(server);

  // main loop
  while (1) {
    buffer = s_recv(server->socket);
    if (buffer == NULL) ebmq_error("ebmq_server_listen");
    puts("got message");
    ebmq_server_reply_ok(server);
    s_console(buffer);
    ebmqd_sleep();
  }

  // ensure successful exit
  exit(EXIT_SUCCESS);
}


/**
 * Stops the daemon
 */
void
ebmqd_stop () {
  if (!ebmqd_is_alive()) return;
  ebmq_log("ebmqd: stopping daemon");
  closelog();
  fflush(stdout);
  fflush(stdin);
  fflush(stderr);
  // kill process
  ebmqd_kill();
  //
  if (!ebmqd_clear_pid()) {
    ebmqd_log(LOG_ERR, "failed to remove ebmqd.pid");
    ebmq_error("ebmqd: failed to clear pid");
  }
  // graceul exit
  exit(EXIT_SUCCESS);
}


/**
 * Stops and starts the daemon
 */
void
ebmqd_restart () {
  if (ebmqd_is_alive()) {
    ebmqd_stop();
    ebmqd_start();
  } else {
    ebmq_error("ebmqd: (no pid file) not running");
  }
}

