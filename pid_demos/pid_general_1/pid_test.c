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
 * Simulates a retrieval of a measurement following
 */
float updateTestMeasurement(float pidOutput, float measurement)
{
    // static float output = 0.0f;
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