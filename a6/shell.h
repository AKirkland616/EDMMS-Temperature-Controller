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

int shell_cmd_push(shell_cmd_args*);
int shell_cmd_pop(shell_cmd_args*);
int shell_cmd_sell(shell_cmd_args*);
int shell_cmd_show(shell_cmd_args*);

int cost(box_t*);

/******
 *
 *    CONSTANTS
 *
 ******/
shell_cmds my_shell_cmds = {
  .count = 6,
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
      .cmd  = "push",
      .desc = "add a box of items to the top of the stack",
      .func = shell_cmd_push
    },
    {
      .cmd  = "pop",
      .desc = "remove a box of items from the top of the stack",
      .func = shell_cmd_pop
    },
    {
      .cmd  = "sell",
      .desc = "sell the top box on the stack, showing its cost",
      .func = shell_cmd_sell
    },
    {
      .cmd  = "show",
      .desc = "display the contents of the whole stack",
      .func = shell_cmd_show
    },
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

