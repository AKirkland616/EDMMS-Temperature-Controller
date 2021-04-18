// File:            shell.c
// Author:          Jeremy, Anthony, Macallister, Lorand
// 
// Description:     Implementation of shell.h
//
// =========================================================================================================

#include "msp430_shell.h"

/* Forward declarations ***************************/

int validate_zero_or_one(char *input);
int validate_nonnegative_integer(char *input);

/* MSP430 shell constants *************************/

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

/* MSP430 shell globals ***************************/

PID_t PID;
char cmd_line[90];
int running = 0;

/* Header Implementation **************************/

int shell_cmd_help(shell_cmd_args *args)
{
    int k;

    for(k = 0; k < my_shell_cmds.count; k++) 
    {
        cio_printf("%s: %s\n\r", my_shell_cmds.cmds[k].cmd, my_shell_cmds.cmds[k].desc);
    }

    return 0;
}

int shell_cmd_argt(shell_cmd_args *args)
{
    int k;
    cio_print("Arguments given:\n\r");

    for(k = 0; k < args->count; k++) 
    {
        cio_printf(" - %s\n\r", args->args[k].val);
    }

    return 0;
}

int shell_cmd_run(shell_cmd_args *args)
{
    if(running != 1)
    {
        cio_printf("Running program %i at step %i.\r\n", PID.program, PID.step);
        running = 1;
    } 
    
    else
    {
        cio_printf("Program is already running.\n\r");
    }

    return 0;
}

int shell_cmd_pause(shell_cmd_args *args)
{
    if((running != 1))
    {
        cio_printf("Program must be running to pause it.\r\n");
    }
    else
    {
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
    } 
    else
    {
        cio_printf("Program is not running or is paused.\r\n");
    }

    return 0;
}

int shell_cmd_loadprogram(shell_cmd_args *args)
{
    int valid = validate_nonnegative_integer(args->args[0].val);
    if (valid == 0)
    {
        cio_printf("Loading program %s\r\n", args->args[0].val);
        PID.program = atoi(args->args[0].val);
    } 
    else 
    {
        cio_printf("Program not loaded. Value must be a non-negative integer.\r\n");
    }

    return 0;
}

int shell_cmd_loadstep(shell_cmd_args *args)
{
    int valid = validate_nonnegative_integer(args->args[0].val);
    if (valid == 0) 
    {
        cio_printf("Loading step %s.\r\n", args->args[0].val);
        PID.step = atoi(args->args[0].val);
    } 
    else 
    {
        cio_printf("Step not loaded. Value must be a non-negative integer.\r\n");
    }

    return 0;
}

int shell_cmd_setpoint(shell_cmd_args *args)
{
    int valid = validate_nonnegative_integer(args->args[0].val);
    if (valid == 0) 
    {
        cio_printf("Setting target setpoint to %s Kelvins.\r\n", args->args[0].val);
        PID.setpoint = atoi(args->args[0].val);
    } 
    else 
    {
        cio_printf("Setpoint not set for the current step. Value must be a non-negative integer (Kelvins).\r\n");
    }

    return 0;
}

int shell_cmd_setstepinterval(shell_cmd_args *args)
{ 
    int valid = validate_nonnegative_integer(args->args[0].val);
    if (valid == 0) 
    {
        cio_printf("Setting interval for step %i to %s minutes.\r\n", PID.step, args->args[0].val);
        PID.stepinterval = atoi(args->args[0].val);
    } 
    else 
    {
        cio_printf("Step interval not set for step %i. Value must be a non-negative integer (minutes).\r\n", PID.step);
    }
    return 0;
}

int shell_cmd_verbose(shell_cmd_args *args)
{
    int valid = validate_zero_or_one(args->args[0].val);
    if (valid == 0) 
    {
        cio_printf("Setting verbose to %s.\r\n", args->args[0].val);
        PID.v = atoi(args->args[0].val);
    } 
    else 
    {
        cio_printf("Verbose not set. Input is not a zero or one. Must be a one digit representation.\r\n");
    }

    return 0;
}

int shell_cmd_setreportinterval(shell_cmd_args *args)
{
    int valid = validate_nonnegative_integer(args->args[0].val);
    if (valid == 0) 
    {
        cio_printf("Setting reporting interval to %s seconds.\r\n", args->args[0].val);
        PID.reportinterval = atoi(args->args[0].val);
    } 
    else 
    {
        cio_printf("Reporting interval not set. Value must be a non-negative integer (seconds).\r\n");
    }

    return 0;
}

int shell_cmd_PID(shell_cmd_args *args)
{
    cio_printf("Setting PID:\r\n");
    int valid_p = validate_nonnegative_integer(args->args[0].val);
    if (valid_p == 0) 
    {
        PID.p = atoi(args->args[0].val);
        cio_printf("\tP = %i,\n\r", PID.p);
    } 
    else 
    {
        cio_printf("Proportional term not set. Value must be a non-negative integer.\r\n");
    }

    int valid_i = validate_nonnegative_integer(args->args[1].val);
    if (valid_i == 0) 
    { 
        PID.i = atoi(args->args[1].val);
        cio_printf("\tI = %i,\n\r", PID.i);
    } 
    else 
    {
        cio_printf("Integral term not set. Value must be a non-negative integer.\r\n");
    }

    int valid_d = validate_nonnegative_integer(args->args[2].val);
    if (valid_d == 0) 
    {
        PID.d = atoi(args->args[2].val);
        cio_printf("\tD = %i\n\r", PID.d);
    } 
    else 
    {
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

/* Shell input validation *************************/

int validate_zero_or_one(char *input) 
{
    if (strlen(input) != 1 || (strncmp(input, "0", (size_t) 1) != 0 && strncmp(input, "1", (size_t) 1) != 0)) {
        return -1;
    }

    return 0;
}

int validate_nonnegative_integer(char *input)
{
    for(int i = 0; input[i] != '\0'; i++) 
    {
        if(!(isdigit(input[i]))) 
        {
            return -1;
        }
    }
    return 0;
} 
 