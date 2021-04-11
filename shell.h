#ifndef __SHELL_H__
#define __SHELL_H__

#include <libemb/shell/shell.h>

/******
 *
 *    STRUCTURES
 *
 ******/

typedef struct _PID_t {
    int p;
    int i;
    int d;
    int v;
    int program;
    int step;
    int setpoint;
    int stepinterval;
    int reportinterval;
} PID_t;

/******
 *
 *    PROTOTYPES
 *
 ******/
int shell_cmd_help(shell_cmd_args *args);
int shell_cmd_argt(shell_cmd_args *args);
int shell_cmd_run(shell_cmd_args *args);
int shell_cmd_pause(shell_cmd_args *args);
int shell_cmd_stop(shell_cmd_args *args);
int shell_cmd_loadprogram(shell_cmd_args *args);
int shell_cmd_loadstep(shell_cmd_args *args);
int shell_cmd_setpoint(shell_cmd_args *args);
int shell_cmd_setstepinterval(shell_cmd_args *args);
int shell_cmd_verbose(shell_cmd_args *args);
int shell_cmd_setreportinterval(shell_cmd_args *args);
int shell_cmd_PID(shell_cmd_args *args);
int shell_cmd_show(shell_cmd_args *args);
int validate_nonnegative_integer(char *input);
int validate_zero_or_one(char *input);

/******
 *
 *    CONSTANTS
 *
 ******/
shell_cmds my_shell_cmds = {
  .count = 13,
  .cmds  = {
    {
      .cmd  = "help",
      .desc = "List available commands.",
      .func = shell_cmd_help
    },
    {
      .cmd  = "PID",
      .desc = "Set 3 values for PID, respectively.",
      .func = shell_cmd_PID
    },
    {
      .cmd  = "args",
      .desc = "Print back given arguments.",
      .func = shell_cmd_argt
    },
    {
      .cmd  = "+",
      .desc = "Run the currently selected program from the currently selected step.",
      .func = shell_cmd_run
    },
    {
      .cmd  = "|",
      .desc = "Pause execution and maintain the current setpoint.",
      .func = shell_cmd_pause
    },
    {
      .cmd  = "-",
      .desc = "Halt execution and turn the output pin off.",
      .func = shell_cmd_stop
    },
    {
      .cmd  = "p",
      .desc = "Load program N, step 0.",
      .func = shell_cmd_loadprogram
    },
    {
      .cmd  = "s",
      .desc = "Load step N of the current program.",
      .func = shell_cmd_loadstep
    },
    {
      .cmd  = ".",
      .desc = "Set the target setpoint of the current step to N Kelvins. (Only allowed when execution is halted.)",
      .func = shell_cmd_setpoint
    },
    {
      .cmd  = "m",
      .desc = "Set the interval of the current step to N minutes.",
      .func = shell_cmd_setstepinterval
    },
    {
      .cmd  = "v",
      .desc = "Unset/Set verbose mode. (Zero or one.)",
      .func = shell_cmd_verbose
    },
    {
      .cmd  = "r",
      .desc = "Set the reporting interval to N seconds.",
      .func = shell_cmd_setreportinterval
    },
    {
      .cmd  = "show",
      .desc = "Display the contents PID object.",
      .func = shell_cmd_show
    },
  }
};

/******
 *
 *    GLOBALS
 *
 ******/

PID_t PID;
char cmd_line[90];
int running = 0;

#endif
