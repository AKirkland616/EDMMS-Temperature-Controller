#include <msp430.h>
#include <stdlib.h>
#include <string.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include <libemb/shell/shell.h>
#include "shell.h"
/******
 *
 *      GLOBAL VARIABLES
 *
 ******/

char cmd[90] = {0};

/******
 *
 *      CALLBACK HANDLERS
 *
 ******/
int shell_cmd_help(shell_cmd_args *args)
{
  int k;

  for(k = 0; k < my_shell_cmds.count; k++) {
    cio_printf("%s: %s\n\r", my_shell_cmds.cmds[k].cmd, my_shell_cmds.cmds[k].desc);
  }

  return 0;
}

int shell_cmd_argt(shell_cmd_args *args)
{
  int k;

  cio_print("args given:\n\r");

  for(k = 0; k < args->count; k++) {
    cio_printf(" - %s\n\r", args->args[k].val);
  }

  return 0;
}

int shell_cmd_run_this_step(shell_cmd_args *args)
{
  cio_printf("%s\r\n", "RUN THIS STEP");

  return 0;
}

int shell_cmd_pause_execution(shell_cmd_args *args)
{
  cio_printf("%s\r\n", "PAUSE EXECUTION");

  return 0;
}

int shell_cmd_halt_execution(shell_cmd_args *args)
{
  cio_printf("%s\r\n", "HALT EXECUTION");

  return 0;
}

int shell_cmd_load_program(shell_cmd_args *args)
{
  cio_printf("%s\r\n", "LOAD PROGRAM");

  return 0;
}

int shell_cmd_load_step(shell_cmd_args *args)
{
  cio_printf("%s\r\n", "LOAD STEP");

  return 0;
}

int shell_cmd_set_target_setpoint(shell_cmd_args *args)
{
  cio_printf("%s\r\n", "SET TARGET SETPOINT");

  return 0;
}

int shell_cmd_set_interval(shell_cmd_args *args)
{
  cio_printf("%s\r\n", "SET INTERVAL");

  return 0;
}

int shell_cmd_reset_set_verbose(shell_cmd_args *args)
{
  cio_printf("%s\r\n", "RESET/SET VERBOSE");

  return 0;
}

int shell_cmd_set_reporting_interval(shell_cmd_args *args)
{
  cio_printf("%s\r\n", "SET REPORTING INTERVAL");

  return 0;
}

int shell_process(char *cmd_line)
{
  return shell_process_cmds(&my_shell_cmds, cmd_line);
}

/******
 *
 *      INITIALIZATION
 *
 ******/
int main(void)
{
  /* CLOCKS *********************************/
  WDTCTL   = WDTPW | WDTHOLD;               // Disable Watchdog
  BCSCTL1  = CALBC1_1MHZ;                   // Run @ 1MHz
  DCOCTL   = CALDCO_1MHZ;

  /* GPIO ***********************************/
  P1DIR  = ~BIT3;
  P1DIR |=  BIT0 | BIT6;
  P1OUT  =  BIT3;

  P1REN = BIT3;
  P1IE  = BIT3;
  P1IES = BIT3;
  P1IFG = 0;

  P2DIR = -1;
  P2OUT = 0;

  serial_init(9600);                        // Initialize Serial Comms
  __eint();                                 // Enable Global Interrupts

/******
 *
 *      PROGRAM LOOP
 *
 ******/
  for (;;) {
    int j = 0;                              // Char array counter
    memset(cmd_line, 0, 90);                // Init empty array

    cio_print("$ ");                        // Display prompt
    char c = cio_getc();                    // Wait for a character
    while(c != '\r') {                      // until return sent then ...
      if(c == 0x08) {                       //  was it the delete key?
        if(j != 0) {                        //  cursor NOT at start?
          cmd_line[--j] = 0;                //  delete key logic
          cio_printc(0x08); 
          cio_printc(' '); 
          cio_printc(0x08);
        }
      } else {                              // otherwise ...
        cmd_line[j++] = c;
        cio_printc(c);                      //  echo received char
      }
      c = cio_getc();                       // Wait for another
    }

    cio_print("\n\n\r");                    // Delimit command result

    switch(shell_process(cmd_line))         // Execute specified shell command
    {                                       // and handle any errors
      case SHELL_PROCESS_ERR_CMD_UNKN:
        cio_print("ERROR, unknown command given\n\r");
        break;
      case SHELL_PROCESS_ERR_ARGS_LEN:
        cio_print("ERROR, an argument is too lengthy\n\r");
        break;
      case SHELL_PROCESS_ERR_ARGS_MAX:
        cio_print("ERROR, too many arguments given\n\r");
        break;
      default:
        break;
    }

    cio_print("\n");               // Delimit Result
  }

  return 0;
}

