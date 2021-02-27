#ifndef __SHELL_H__
#define __SHELL_H__

#include <libemb/shell/shell.h>

/******
 *
 *    STRUCTURES
 *
 ******/

typedef struct _box_t {
    char* name;
    int quantity;
    int price;
} box_t;

/******
 *
 *    PROTOTYPES
 *
 ******/
int shell_cmd_help(shell_cmd_args*);
int shell_cmd_argt(shell_cmd_args*);

int shell_cmd_run_this_step(shell_cmd_args*);
int shell_cmd_pause_execution(shell_cmd_args*);
int shell_cmd_halt_execution(shell_cmd_args*);
int shell_cmd_load_program(shell_cmd_args*);
int shell_cmd_load_step(shell_cmd_args*);
int shell_cmd_set_target_setpoint(shell_cmd_args*);
int shell_cmd_set_interval(shell_cmd_args*);
int shell_cmd_reset_set_verbose(shell_cmd_args*);
int shell_cmd_set_reporting_interval(shell_cmd_args*);

int cost(box_t*);

/******
 *
 *    CONSTANTS
 *
 ******/
shell_cmds my_shell_cmds = {
  .count = 11,
  .cmds  = {
    {
      .cmd  = "help",
      .desc = "list available commands",
      .func = shell_cmd_help
    },
    {
      .cmd  = "args",
      .desc = "print back given arguments",
      .func = shell_cmd_argt
    },
    {
      .cmd  = "+",
      .desc = "run the currently selected program from the currently selected step",
      .func = shell_cmd_run_this_step
    },
    {
      .cmd = "|",
      .desc = "pause execution and maintain the current setpoint",
      .func = shell_cmd_pause_execution
    },
    {
      .cmd = "-",
      .desc = "halt execution and turn the output pin off",
      .func = shell_cmd_halt_execution
    },
    {
      .cmd = "p",
      .desc = "load program",
      .func = shell_cmd_load_program
    },
    {
      .cmd = "s",
      .desc = "load step of the current program",
      .func = shell_cmd_load_step
    },
    {
      .cmd = ".",
      .desc = "set the target setpoint of the current step to N Kelvins",
      .func = shell_cmd_set_target_setpoint
    },
    {
      .cmd = "m",
      .desc = "set the interval of the current step",
    .func = shell_cmd_set_interval
    },
    {
      .cmd = "v",
      .desc = "reset/set verbose mode",
      .func = shell_cmd_reset_set_verbose
    },
    {
      .cmd = "r",
      .desc = "set reporting interval",
      .func = shell_cmd_set_reporting_interval
    }
  }
};

/******
 *
 *    GLOBALS
 *
 ******/

box_t* stack[5];
int stack_top = 0;
char cmd_line[90];

#endif

