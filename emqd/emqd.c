/* std includes */
#include <stdlib.h>
#include <assert.h>

/* emq includes */
#include "emq/util.h"
#include "emqd.h"
#include "commander.h"

static void
verbose_opt (command_t *self) {
  emq_enable_verbose();
  emq_verbose("showing verbose output");
}

int
main (int argc, char *argv[]) {
  command_t program;

  /**
   * we need to figure out the
   * action being taken on the
   * daemon process
   */
  if (emqd_has_command(argv, "start")) {
    if (emqd_is_alive()) {
      emq_error("daemon is already running");
    } else {
      emqd_start();
    }
  } else if (emqd_has_command(argv, "stop")) {
    if (emqd_is_alive()) {
      emqd_stop();
    } else {
      emq_error("daemon not running");
    }
  }
  else if (emqd_has_command(argv, "restart")) {
    emqd_restart();
  }

  // initialize program setting program version
  command_init(&program, "emqd", EMQ_VERSION);

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
emqd_start () {
  pid_t pid, sid;
  if (emqd_is_root() == 0) {
    emq_error("Operation not permitted");
  }

  /** 
   * Flush stdout, stdin, and stderr
   * so forked process does not inherit
   * streams
   */
  fflush(stdout);
  fflush(stdin);
  fflush(stderr);

  /**
   * set the pid and exit with failure if it
   * failed to return one greater than `0`
   * else exit with success
   */
  pid = fork();
  if (pid < 0) {
    emq_error("Failed to fork and get `pid`");
  }

  if (pid > 0) {
    // write pid to file
    emqd_write_pid(pid);
    emqd_set_opt(EMQD_CHILD, 1);
    exit(EXIT_SUCCESS);
  }

  emqd_set_pid(pid);

  // ensure the pid was set internally
  assert(emqd_get_pid() > 0);

  // set the unmask to `0`
  umask(0);

  /**
   * set the sid and exit with failure if
   * it failed to return one greater than `0`
   */
  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  emqd_set_sid(sid);

  // ensure the sid was set internally
  assert(emqd_get_sid() > 0);

  /**
   * set root working directory
   * and exit with a failure if
   * it failed to return greater
   * than `0`
   */
  if (chdir("/") < 0) {
    exit(EXIT_FAILURE);
  }

  emq_info("starting daemon..");

  // close all file descriptors
  emqd_close_fds();

  // open logs
  emqd_open_logs();

  // main loop
  while (1) {
    // magic
    emqd_sleep();
  }

  // ensure successful exit
  exit(EXIT_SUCCESS);
}


/**
 * Stops the daemon
 */
void
emqd_stop () {
  if (!emqd_is_alive()) return;
  emq_info("stoping daemon");
  closelog();
  fflush(stdout);
  fflush(stdin);
  fflush(stderr);
  // kill process
  emqd_kill();
  //
  if (!emqd_clear_pid()) {
    emq_error("failed to clear pid");
  }
  // graceul exit
  exit(EXIT_SUCCESS);
}


/**
 * Stops and starts the daemon
 */
void
emqd_restart () {
  if (emqd_is_alive()) {
    emqd_stop();
    emqd_start();
  } else {
    emq_error("daemon not running");
  }
}

