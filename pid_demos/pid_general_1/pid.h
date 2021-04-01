// Program:     Pid_General_1
// File:        pid.h
// Author:      Jeremy Evans
//
// Description: Declares data structures and functions for a generic PID
//              controller
//
// ==============================================================================

#ifndef PID_GENERAL
#define PID_GENERAL

#include "pid_test_constants.h"

/**
 * Contains all of the necessary data for a single instance of a PID controller.
 * 
 * @param proportionalGain The proportional gain.
 * @param integralGain The integral gain.
 * @param derivativeGain The derivative gain.
 * @param lpfconstant Derivative low-pass filter time constant.
 * @param outputMin Minimum output value.
 * @param outputMax Maximum output value.
 * @param samplingTime Controller sampling time (in seconds).
 * @param integrator Output of the integral term.
 * @param prevError The value of the previous error (needed for the integral term).
 * @param differentiator Output of the derivative term.
 * @param prevMeasurement The previous read measurement (needed for the derivative term).
 * @param output Controller output
 */
typedef struct __PIDController {
    float proportionalGain;
    float integralGain;
    float derivativeGain;

    float lpfconstant;

    float outputMin;
    float outputMax;

    float samplingTime;

    float integrator;
    float prevError;
    float differentiator;
    float prevMeasurement;

    float output;

} PIDController;

/**
 * Initializes a new pid controller with default values.
 */
PIDController* PID_Init();

/**
 * Computes the controller output based on a given setpoint and measurement.
 * 
 * @param pid The controller to compute the output of.
 * @param setpoint The desired value to arrive at.
 * @param measurement The measured value (the error is the difference between the setpoint and the measurement)
 * 
 * @return The controller output.
 */
float PID_Update(PIDController* pid, float setpoint, float measurement);

# endif