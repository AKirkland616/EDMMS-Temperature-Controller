#include "pid.h"

void PID_Init(PIDController* pid)
{
    if (pid == NULL)
    {
        pid = (PIDController*)malloc(sizeof(PIDController));
    }

    pid->integrator = 0.0f;
    pid->prevError = 0.0f;

    pid->differentiator = 0.0f;
    pid->prevMeasurement = 0.0f;

    pid->output = 0.0f;
}

float PID_Update(PIDController* pid, float setpoint, float measurement)
{
    float error = setpoint - measurement;

    // Compute output from proportional term
    float proportional = pid->proportionalGain * error;
    
    // Compute output from integral term
    pid->integrator += + 0.5f * pid->integralGain * pid->samplingTime * (error + pid->prevError);

    // Calculate integrator limits
    float limMaxInt = pid->outputMax > proportional
        ? pid->outputMax - proportional
        : 0.0f;

    float limMinInt = pid->outputMin < proportional
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
