#include <msp430.h>
#include <stdlib.h>
#include <string.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include <libemb/shell/shell.h>
#include "shell.h"
#include <ctype.h>
/******
 *
 *      GLOBAL VARIABLES
 *
 ******/

char cmd[90] = {0};

/******
 *
 *      SHELL COMMANDS
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
  cio_print("Arguments given:\n\r");
  for(k = 0; k < args->count; k++) {
    cio_printf(" - %s\n\r", args->args[k].val);
  }

  return 0;
}

int shell_cmd_run(shell_cmd_args *args)
{
  if(running != 1){
    cio_printf("Running program %i at step %i.\r\n", PID.program, PID.step);
    running = 1;
  } else{
    cio_printf("Program is already running.\n\r");
  }
  
  return 0;
}

int shell_cmd_pause(shell_cmd_args *args)
{
  if((running != 1))
  {
    cio_printf("Program must be running to pause it.\r\n");
  }else{
    cio_printf("Pausing program %i at step %i.\r\n", PID.program, PID.step);
    running = 2;
  }

  return 0;
}

int shell_cmd_stop(shell_cmd_args *args)
{
  if(running != 0){
    cio_printf("Stopping program %i at step %i.\r\n", PID.program, PID.step);
    running = 0;
  } else{
    cio_printf("Program is not running or is paused.\r\n");
  }
  
  return 0;
}

int shell_cmd_loadprogram(shell_cmd_args *args)
{
  int valid = validate_nonnegative_integer(args->args[0].val);
  if (valid == 0) {
    cio_printf("Loading program %s\r\n", args->args[0].val);
    PID.program = atoi(args->args[0].val);
  } else {
    cio_printf("Program not loaded. Value must be a non-negative integer.\r\n");
  }

  return 0;
}

int shell_cmd_loadstep(shell_cmd_args *args)
{
  int valid = validate_nonnegative_integer(args->args[0].val);
  if (valid == 0) {
    cio_printf("Loading step %s.\r\n", args->args[0].val);
    PID.step = atoi(args->args[0].val);
  } else {
    cio_printf("Step not loaded. Value must be a non-negative integer.\r\n");
  }

  return 0;
}

int shell_cmd_setpoint(shell_cmd_args *args)
{
  int valid = validate_nonnegative_integer(args->args[0].val);
  if (valid == 0) {
    cio_printf("Setting target setpoint to %s Kelvins.\r\n", args->args[0].val);
    PID.setpoint = atoi(args->args[0].val);
  } else {
    cio_printf("Setpoint not set for the current step. Value must be a non-negative integer (Kelvins).\r\n");
  }

  return 0;
}

int shell_cmd_setstepinterval(shell_cmd_args *args)
{ 
  int valid = validate_nonnegative_integer(args->args[0].val);
  if (valid == 0) {
    cio_printf("Setting interval for step %i to %s minutes.\r\n", PID.step, args->args[0].val);
    PID.stepinterval = atoi(args->args[0].val);
  } else {
    cio_printf("Step interval not set for step %i. Value must be a non-negative integer (minutes).\r\n", PID.step);
  }
  return 0;
}

int shell_cmd_verbose(shell_cmd_args *args)
{
  int valid = validate_zero_or_one(args->args[0].val);
  if (valid == 0) {
    cio_printf("Setting verbose to %s.\r\n", args->args[0].val);
    PID.v = atoi(args->args[0].val);
  } else {
    cio_printf("Verbose not set. Input is not a zero or one. Must be a one digit representation.\r\n");
  }
  
  return 0;
}

int shell_cmd_setreportinterval(shell_cmd_args *args)
{
  int valid = validate_nonnegative_integer(args->args[0].val);
  if (valid == 0) {
    cio_printf("Setting reporting interval to %s seconds.\r\n", args->args[0].val);
    PID.reportinterval = atoi(args->args[0].val);
  } else {
    cio_printf("Reporting interval not set. Value must be a non-negative integer (seconds).\r\n");
  }

  return 0;
}

int shell_cmd_PID(shell_cmd_args *args)
{
  cio_printf("Setting PID:\r\n");
  int valid_p = validate_nonnegative_integer(args->args[0].val);
  if (valid_p == 0) {
    PID.p = atoi(args->args[0].val);
    cio_printf("\tP = %i,\n\r", PID.p);
  } else {
    cio_printf("Proportional term not set. Value must be a non-negative integer.\r\n");
  }

  int valid_i = validate_nonnegative_integer(args->args[1].val);
  if (valid_i == 0) { 
    PID.i = atoi(args->args[1].val);
    cio_printf("\tI = %i,\n\r", PID.i);
  } else {
    cio_printf("Integral term not set. Value must be a non-negative integer.\r\n");
  }

  int valid_d = validate_nonnegative_integer(args->args[2].val);
  if (valid_d == 0) {
    PID.d = atoi(args->args[2].val);
    cio_printf("\tD = %i\n\r", PID.d);
  } else {
    cio_printf("Derivative term not set. Value must be a non-negative integer.\r\n");
  }

  return 0;
}

int shell_cmd_show(shell_cmd_args *args)
{
  cio_printf("\tP:\t\t\t%i\r\n", PID.p);
  cio_printf("\tI:\t\t\t%i\r\n", PID.i);
  cio_printf("\tD:\t\t\t%i\r\n", PID.d);
  cio_printf("\tverbose:\t\t%i\r\n", PID.v);
  cio_printf("\tprogram:\t\t%i\r\n", PID.program);
  cio_printf("\tstep:\t\t\t%i\r\n", PID.step);
  cio_printf("\tset point:\t\t%i\r\n", PID.setpoint);
  cio_printf("\tstep interval:\t\t%i\r\n", PID.stepinterval);
  cio_printf("\treport interval:\t%i\r\n", PID.reportinterval);
  
  return 0;
}

int shell_process(char *cmd_line)
{
  return shell_process_cmds(&my_shell_cmds, cmd_line);
}

/******
 *
 *	SHELL INPUT VALIDATION
 *
 ******/
int validate_zero_or_one(char *input) {
  if (strlen(input) != 1 || (strncmp(input, "0", (size_t) 1) != 0 && strncmp(input, "1", (size_t) 1) != 0)) {
    return -1;
  }

  return 0;
}

int validate_nonnegative_integer(char *input)
{
  for(int i = 0; input[i] != '\0'; i++) {
    if(!(isdigit(input[i]))) {
      return -1;
    }
  }
  return 0;
}  

/******
 *
 *      INITIALIZATION
 *
 ******/
int main(void)
{
  
  PID.p = 0;
  PID.i = 0;
  PID.d = 0;
  PID.v = 1;
  PID.program = 0;
  PID.step = 0;
  PID.setpoint = 0;
  PID.stepinterval = 0;
  PID.reportinterval = 0;

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

/******
 *
 *      INTERRUPTS
 *
 ******/
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR (void)
{
  // stop running program when the button is pressed

  while (!(BIT3 & P1IN)) {} // is finger off of button yet?
  __delay_cycles(32000);    // wait 32ms
  P1IFG &= ~BIT3;           // clear interrupt flag

  running = 0;
}
