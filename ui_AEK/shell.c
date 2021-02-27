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

int shell_cmd_run(shell_cmd_args *args)
{
  if(running != 1){
    cio_printf("Running Program %i at step %i\n\r", PID.program, PID.step);
    running = 1;
  } else{
    cio_printf("Program is already running\n\r");
  }
  
  return 0;
}

int shell_cmd_pause(shell_cmd_args *args)
{
  if((running != 1))
  {
    cio_printf("Program must be running to pause it.\n\r");
  }else{
    cio_printf("Pausing Program %i at step %i\n\r", PID.program, PID.step);
    running = 2;
  }
  return 0;
}

int shell_cmd_stop(shell_cmd_args *args)
{
  if(running != 0){
    cio_printf("Stopping Program %i at step %i\n\r", PID.program, PID.step);
    running = 0;
  } else{
    cio_printf("Program is not running or paused.\n\r");
  }
  
  return 0;
}

int shell_cmd_loadprogram(shell_cmd_args *args)
{
  cio_printf("Loading Program %s\n\r", args->args[0].val);
  PID.program = atoi(args->args[0].val);
  
  return 0;
}

int shell_cmd_loadstep(shell_cmd_args *args)
{
  cio_printf("Loading Step %s\n\r", args->args[0].val);
  PID.step = atoi(args->args[0].val);
  
  return 0;
}

int shell_cmd_setpoint(shell_cmd_args *args)
{
  cio_printf("Setting target setpoint to %s Kalvins\n\r", args->args[0].val);
  PID.setpoint = atoi(args->args[0].val);
  
  return 0;
}

int shell_cmd_setstepinterval(shell_cmd_args *args)
{
  cio_printf("Setting interval for step %i to %s\n\r", PID.step, args->args[0].val);
  PID.stepinterval = atoi(args->args[0].val);
  
  return 0;
}

int shell_cmd_verbose(shell_cmd_args *args)
{
  cio_printf("Setting Verbose to %s\n\r", args->args[0].val);
  PID.v = atoi(args->args[0].val);
  
  return 0;
}

int shell_cmd_setreportinterval(shell_cmd_args *args)
{
  cio_printf("Setting Report interval to %s\n\r", args->args[0].val);
  PID.reportinterval = atoi(args->args[0].val);
  
  return 0;
}

int shell_cmd_PID(shell_cmd_args *args)
{
  cio_printf("Setting PID:\r\n");
  PID.p = atoi(args->args[0].val);
  PID.i = atoi(args->args[1].val);
  PID.d = atoi(args->args[2].val);
  cio_printf("\tP = %i,\n\r", PID.p);
  cio_printf("\tI = %i,\n\r", PID.i);
  cio_printf("\tD = %i\n\r", PID.d);
  

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
 *      FUNCTIONS
 *
 ******/



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
