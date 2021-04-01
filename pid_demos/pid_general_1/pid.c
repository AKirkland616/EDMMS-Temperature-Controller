// Program:     Pid_General_1
// File:        pid.c
// Author:      Jeremy Evans
//
// Description: Implementation of pid.h
//
// ==============================================================================

#include <stdlib.h>
#include "pid.h"

PIDController* PID_Init()
{
    PIDController* pid = (PIDController*)malloc(sizeof(PIDController));

    // Initialize process variable components
    pid->integrator = 0.0f;
    pid->prevError = 0.0f;

    pid->differentiator = 0.0f;
    pid->prevMeasurement = 0.0f;

    pid->output = 0.0f;

    // Set PID variables with testing constants
    pid->proportionalGain = TEST_PROPORTIONAL_GAIN;
    pid->integralGain = TEST_INTEGRAL_GAIN;
    pid->derivativeGain = TEST_DERIVATIVE_GAIN;
    pid->lpfconstant = TEST_LPF_CONSTANT;
    pid->outputMin = TEST_OUTPUT_MIN;
    pid->outputMax = TEST_OUTPUT_MAX;
    pid->samplingTime = TEST_SAMPLE_TIME_S;

    return pid;
}

float PID_Update(PIDController* pid, float setpoint, float measurement)
{
    float error = setpoint - measurement;

    // Compute output from proportional term
    float proportional = pid->proportionalGain * error;
    
    // Compute output from integral term
    pid->integrator += + 0.5f * pid->integralGain * pid->samplingTime * (error + pid->prevError);

    // Calculate integrator limits
    float limMaxInt = proportional < pid->outputMax 
        ? pid->outputMax - proportional
        : 0.0f;

    float limMinInt = proportional > pid->outputMin
        ? pid->outputMin - proportional
        : 0.0f;

    // Clamp integrator
    if (pid->integrator > limMaxInt)
        pid->integrator = limMaxInt;
    else if (pid->integrator < limMinInt)
        pid->integrator = limMinInt;

    // Compute output from derivative term (band-limited differentiator)
    pid->differentiator = (2.0f * pid->derivativeGain * (measurement - pid->prevMeasurement)
                        + (2.0f * pid->lpfconstant - pid->samplingTime) * pid->differentiator)
                        / (2.0f * pid->lpfconstant + pid->samplingTime);

    // Compute output and apply limits to result
    pid->output = proportional + pid->integrator + pid->differentiator;
    
    if (pid->output > pid->outputMax)
        pid->output = pid->outputMax;
    else if (pid->output < pid->outputMin)
        pid->output = pid->outputMin;
    
    // Store error and measurement
    pid->prevError = error;
    pid->prevMeasurement = measurement;

    return pid->output;
}
