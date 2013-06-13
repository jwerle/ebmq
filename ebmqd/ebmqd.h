#ifndef EBMQD_H
#define EBMQD_H

/* std include */
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <stdbool.h>

/* ebmq includes */
#include "ebmq/ebmq.h"
#include "ebmq/util.h"
#include "ebmq/server.h"


/**
 * statics
 */

/**
 * Static sleep count in `ms`
 *
 * @api private
 * @type {int}
 */

static int EBMQD_MS_SLEEP = 1000;

/**
 * Location of the `pid` file
 *
 * @api private
 * @
 */

static char *EBMQD_PID_FILE = "/var/run/ebmqd.pid";

/** 
 * Static pid
 *
 * @api private
 * @type {pid_t}
 */

static pid_t ebmqd_pid;

/** 
 * Static sid
 *
 * @api private
 * @type {pid_t}
 */

static pid_t ebmqd_sid;


/**
 * EBMQD opts
 */

struct ebmqd_opts {
  int IS_CHILD;
};

// define `OPTS` struct
static struct ebmqd_opts OPTS;

/**
 * Public `IS_CHILD` flag
 */

#define EBMQD_CHILD 0


/**
 * ebmqd functions
 */

/**
 * Sleeps a the set internal count in `ms`
 *
 * @api public
 * @return {void}
 */

void
ebmqd_sleep () {
  s_sleep(EBMQD_MS_SLEEP);
}

/**
 * Sets the internal sleep count in `ms`
 *
 * @api public
 * @return {void}
 * @param {int} ms
 */

void
ebmqd_set_sleep (int ms) {
  EBMQD_MS_SLEEP = ms;
}

/**
 *  Sets the internal daemon pid
 *
 * @api public
 * @return {pid_t} *pid
 * @param {pid_t} *pid
 */

void
*ebmqd_set_pid (pid_t pid) {
  ebmqd_pid = pid;
  return &ebmqd_pid;
}

/**
 * Gets the internal daemon pid
 *
 * @api public
 * @return {pid_t} *pid
 */

pid_t
*ebmqd_get_pid () {
  return &ebmqd_pid;
}

/**
 * Sets the internal unique session id (SID)
 *
 * @api public
 * @return {pid_t} sid
 * @param {pid_t} *sid
 */

pid_t
*ebmqd_set_sid (pid_t sid) {
  ebmqd_sid = sid;
  return &ebmqd_sid;
}

/**
 * Gets the internal unique session id (SID)
 *
 * @api public
 * @return {pid_t} *sid
 */

pid_t
*ebmqd_get_sid () {
  return &ebmqd_sid;
}

/**
 * Closes the stdin, stdout, and stderr file descriptors
 *
 * @api public
 * @return {void}
 */

void
ebmqd_close_fds () {
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
}

/**
 * Checks if the given char array has
 * the command as the leading argument
 *
 * @api public
 * @return {int}
 * @param {char} *argv
 * @param {char} *command
 */

int
ebmqd_has_command (char *argv[], char *command) {
  if (strncmp(argv[1], command, sizeof(&command)) == 0) return 1;
  else return 0;
}

/**
 * Opens the log files for writing
 *
 * @api public
 * @return {void}
 * @param {char} *identity
 */

void
ebmqd_open_logs () {
  openlog("ebmqd", LOG_NOWAIT|LOG_PID, LOG_USER);
}

/**
 * Sets an internal option
 *
 * @api public
 * @return {void}
 * @param {int const} opt
 * @param {int} value
 */

void
ebmqd_set_opt (int const opt, int value) {
  switch (opt) {
    case EBMQD_CHILD:
      OPTS.IS_CHILD = value;
    break;
  }
}

/**
 * Gets an internal option
 *
 * @ api public
 * @return {int}
 * @param {int const} opt
 */

int
ebmqd_get_opt (int const opt) {
  switch (opt) {
    case EBMQD_CHILD:
      return OPTS.IS_CHILD;
    break;

    default: return 0;
  }

  return 0;
}

/**
 * Checks if user is root
 *
 * @api public
 * @return {int}
 */

int
ebmqd_is_root () {
  uid_t uid = getuid(), euid = geteuid();
  if (getuid()) return 0;
  else if (geteuid()) return 0;
  else return 1;
}

/**
 * Writes pid to file
 *
 * @api public
 * @return {void}
 */

void 
ebmqd_write_pid (pid_t pid) {
  FILE *fp = fopen(EBMQD_PID_FILE, "w+");
  fprintf(fp, "%d\n", pid);
  fclose(fp);
}

/**
 * Gets the active daemon pid
 *
 * @api public
 * @return {int}
 */

pid_t
ebmqd_read_pid () {
  if (access(EBMQD_PID_FILE, F_OK) < 0) return 0;
  pid_t pid = 0;
  FILE *fp = fopen(EBMQD_PID_FILE, "r");
  fscanf(fp, "%d\n", &pid);
  return pid;
}

/**
 * Kills the daemon process
 *
 * @api public
 * @return {int}
 */

int
ebmqd_kill () {
  pid_t pid = ebmqd_read_pid();
  if (pid <= 0) return 0;
  else if (!kill(pid, 9)) return 0;
  else return 1;
}

/**
 * Clears the pid file
 *
 * @api public
 * @return {void}
 */

int
ebmqd_clear_pid () {
  if (unlink(EBMQD_PID_FILE) == 0) return 1;
  else return 0;
}

/**
 * Checks if daemon is running
 *
 * @api public
 * @return {int}
 */

int
ebmqd_is_alive () {
  pid_t pid = ebmqd_read_pid();
  if (pid <= 0) return 0;
  kill(pid, 0);
  if (errno == ESRCH) return 0;
  else return 1;
}

/**
 * Writes to log file
 *
 * @api public
 * @return {void}
 * @param {int} level
 * @param {char} *message
 */

void
ebmqd_log (int level, char *message) {
  syslog(level, "%s", message);
}


/**
 * prototypes
 */

 /**
 * Initializes ebmqd
 *
 * @api public
 * @return {void}
 */

void
ebmqd_init () {
 ebmqd_set_opt(EBMQD_CHILD, 0);
}

/**
 * Starts the daemon and performs these tasks:
 * - forks parent process
 * - set umask to `0`
 * - open logs for writing
 * - setting unique SID
 * - change to `/` directory
 * - closes standard file descriptors
 *
 *
 * @api public
 * @return {void}
 */

void
ebmqd_start ();

/**
 * Stops the daemon
 *
 * @api public
 * @return {void}
 */

void
ebmqd_stop ();

/**
 * Stops and restarts the daemon
 *
 * @api public
 * @return {void}
 */

void
ebmqd_restart ();


#endif