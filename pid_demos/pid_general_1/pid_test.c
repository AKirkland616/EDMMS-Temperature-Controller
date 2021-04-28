// Program:     Pid_General_1
// File:        pid_test.c
// Author:      Jeremy Evans
//
// Description: Tests the PID implementation.
//
// ==============================================================================

/* Maximum run-time of simulation */
#define SIMULATION_TIME_MAX 4.0f

#include <stdio.h>
#include <stdlib.h>

#include "pid.h"

/**
 * Emulates a plant process mapping the output from the simulated PID controller
 * (the control variable) to the resulting measurement taken from the controlled
 * system (the process variable).
 * 
 * @param pidOutput The output of the PID controller at the previous moment in time.
 * @param measurement The measurement of the system taken at the previous moment in time.
 * 
 * @returns The measurement of the system taken at the current moment in time.
 */
float updateTestMeasurement(float pidOutput, float measurement)
{
    static const float alpha = 0.02f;

    measurement = (TEST_SAMPLE_TIME_S * pidOutput + measurement) / (1.0f + alpha * TEST_SAMPLE_TIME_S);

    return measurement;
}

int main(int argc, char **argv)
{
    PIDController *pid = PID_Init();

    // Set desired value
    float setpoint = 1.0f;

    // Initialize measurement to 0 (at time = 0, no measurement is taken)
    float measurement = 0.0f;

    printf("Setpoint: %f\n\nTime (s)\tSystem Output\tController Output\r\n", setpoint);
    for (float t = 0.0f; t <= SIMULATION_TIME_MAX; t += TEST_SAMPLE_TIME_S)
    {
        // Get measurement from system
        measurement = updateTestMeasurement(pid->output, measurement);

        // Compute new input signal
        PID_Update(pid, setpoint, measurement);

        printf("%f\t%f\t%f\r\n", t, measurement, pid->output);
    }

    free(pid);

    return 0;
}