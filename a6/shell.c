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

int shell_cmd_push(shell_cmd_args *args)
{
  if (stack_top == 5) {
    cio_printf("%s\r\n", "STACK IS FULL");
  } else {
    cio_printf("BOX STORED: %s\r\n", args->args[0].val);
    stack[stack_top] = malloc(sizeof(box_t));  
    stack[stack_top]->name = malloc(strlen(args->args[0].val));
    strcpy(stack[stack_top]->name, args->args[0].val);
    stack[stack_top]->quantity = atoi(args->args[1].val);
    stack[stack_top]->price = atoi(args->args[2].val);

    stack_top++;
    if (stack_top == 5) {
      P1OUT |=  BIT0;
      P1OUT &= ~BIT6;
    } else {
      P1OUT |=  BIT6;
    }
  }
  return 0;
}

int shell_cmd_pop(shell_cmd_args *args)
{

  if (!stack_top) {
    cio_printf("%s\r\n", "STACK IS EMPTY");
  } else {
    cio_printf("BOX UTTERLY DESTROYED: %s\r\n", stack[stack_top - 1]->name);

    free(stack[stack_top - 1]);
    stack_top--;
  }
  if (!stack_top) 
  {
    P1OUT &= ~BIT6;
  } else {
    P1OUT &= ~BIT0;
    P1OUT |=  BIT6;
  }
  return 0;
}

int shell_cmd_sell(shell_cmd_args *args)
{
  if (!stack_top) {
    cio_printf("%s\r\n", "STACK IS EMPTY");
  } else {

    int the_cost = cost(stack[stack_top - 1]);
    cio_printf("YOU JUST MADE %i CENTS.\r\n", the_cost);

    free(stack[stack_top - 1]);
    stack_top--; 
  }
  if (!stack_top)  {
    P1OUT &= ~BIT6;
  } else {
    P1OUT &= ~BIT0;
    P1OUT |=  BIT6;
  }
  return 0;
}

int shell_cmd_show(shell_cmd_args *args)

{
  if (!stack_top) {                 // check for  empty stack
    cio_printf("%s\r\n", "STACK IS EMPTY");
  } else {
    for (int i = stack_top - 1; i >= 0; i--) {  // print stuff
      cio_printf("NAME:            %s\r\n", stack[i]->name);
      cio_printf("QUANTITY:        %i\r\n", stack[i]->quantity);
      cio_printf("PRICE:           %i\r\n\r\n", stack[i]->price);
    }
    cio_printf(  "NUMBER OF ITEMS: %i\r\n", stack_top);
  }
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

int cost(box_t* box) {

  return box->quantity * box->price;
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

/******
 *
 *      INTERRUPTS
 *
 ******/
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR (void)
{
  // swap the top two boxes on the stack when the button is pressed

  while (!(BIT3 & P1IN)) {} // is finger off of button yet?
  __delay_cycles(32000);    // wait 32ms
  P1IFG &= ~BIT3;           // clear interrupt flag

  if (stack_top < 2) {      // need 2 boxes to swap
    cio_printf("%s", "\r\n\r\nNOT ENOUGH ITEMS\r\n\r\n$ ");
  } else {
    box_t *box = malloc(sizeof(box_t));           // allocate memory
    box = stack[stack_top - 2];                   // s 
    stack[stack_top - 2] = stack[stack_top - 1];  // w
    stack[stack_top - 1] = box;                   // a
  }                                               // p    
}
