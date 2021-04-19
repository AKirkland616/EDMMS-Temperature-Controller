// File:            shell.h
// Author:          Jeremy, Anthony, Macallister, Lorand
// 
// Description:     Declares methods for a shell interface on the MSP430. Input is sent through UART (via
//                  an interface like minicom).
//
// =========================================================================================================

#ifndef MSP430_SHELL
#define MSP430_SHELL

#ifndef __MSP430G2553__
#define __MSP430G2553__
#endif

#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include <libemb/shell/shell.h>
#include <ctype.h>

/* Structs ****************************************/


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

/* MSP430 shell functions *************************/

/**
 * Output the name and description of all available commands.
 */
int shell_cmd_help(shell_cmd_args *args);

/**
 * Output the value of all arguments given.
 * 
 * @param *args The list of arguments given.
 */
int shell_cmd_argt(shell_cmd_args *args);

/**
 * Run the program at a given step (stored internally). If the
 * program is already nothing, it returns without any additional action.
 */
int shell_cmd_run(shell_cmd_args *args);

/**
 * Pauses the program at the current step. If it is not running, no 
 * action is taken.
 */
int shell_cmd_pause(shell_cmd_args *args);

/**
 * Stops the program at the current step. If it is not running, no 
 * action is taken.
 */
int shell_cmd_stop(shell_cmd_args *args);

/**
 * Loads a program identifiable by a non-negative integer.
 * 
 * @param *args A non-negative integer. If the given argument
 * is not a non-negative integer, no action is taken.
 */
int shell_cmd_loadprogram(shell_cmd_args *args);

/**
 * Loads a step (identifiable by a non-negative integer) within
 * a program.
 * 
 * @param *args A non-negative integer. If the given argument
 * is not a non-negative integer, no action is taken.
 */
int shell_cmd_loadstep(shell_cmd_args *args);

/**
 * Sets and stores a given setpoint.
 * 
 * @param *args A non-negative integer representing a desired
 * temperature in Kelvins. If the given argument is not a
 * non-negative integer, no action is taken.
 */
int shell_cmd_setpoint(shell_cmd_args *args);

/**
 * Sets and stores an interval of time between setpoints (in
 * minutes).
 * 
 * @param *args A non-negative integer representing the number
 * of minutes between given setpoints. If the given argument 
 * is not a non-negative integer, no action is taken.
 */
int shell_cmd_setstepinterval(shell_cmd_args *args);

/**
 * Sets and stores the level of detail at which PID data
 * is reported.
 * 
 * @param *args 0 or 1; 0 sets non-verbose reporting, whereas
 * 1 sets verbose reporting. If the given argument is not
 * a 0 or a 1, it sets verbose reporting to 0.
 */
int shell_cmd_verbose(shell_cmd_args *args);

/**
 * Sets and stores the duration of time between which
 * data is reported (in seconds).
 * 
 * @param *args A non-negative integer representing the
 * number of seconds between successive reports. If the 
 * given argument is not a non-negative integer, no action is taken.
 */
int shell_cmd_setreportinterval(shell_cmd_args *args);

/**
 * Sets the proportional, integral, and derivative gains of the
 * PID program.
 * 
 * @param *args Three non-negative integers representing the
 * proportional, integral, and derivative gains. No action is
 * taken for any of the three terms in which a non-negative integer
 * is not given.
 */
int shell_cmd_PID(shell_cmd_args *args);

/**
 * Shows the data currently stored in the PID program.
 */
int shell_cmd_show(shell_cmd_args *args);

/**
 * see /usr/msp430/include/libemb/shell/shell.h : shell_process_cmds
 * 
 * @param   *cmd_line: pointer to command line string.
 * 
 * @return  see /usr/msp430/include/libemb/shell/shell.h : shell_process_cmds
 */
int shell_process(char *cmd_line);

/**
 * Continually get input from the user; designed to run in the main
 * MSP430 program loop.
 */
void shell_get_args();

/**
 * Set the running state. TEST METHOD
 */
void set_running();

/**
 * Set the temperature. TEST METHOD
 * 
 * @param val The temperature read from the MAX31855.
 */ 
void set_temperature(int val);

/* Globals ****************************************/

#endif