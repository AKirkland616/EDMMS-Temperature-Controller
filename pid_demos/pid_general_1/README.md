# PID General Demo 1

## 1 Overview

Contained here is an implementation of PID control and a test function, written within **pid_test.c**, that

* Computes a system measurement at time n based on the measurement and controller output at time *n - T* (*T* is the sample time in seconds; if *n* = 0, both the controller output and the measurement would be zero).

* Computes the PID controller output at time *n* based on the measurement at time *n* and the setpoint (in this program, the setpoint is constant. In practice, the setpoint will change during runtime).

* Increments *n* by sample time *T.*

## 2 Use

### 2.1 Compilation

Compilation is straightforward; after cloning the repo, cd to the repo and run

```bash
make
```

to produce an executable **pid_demo**. 

### 2.2 Execution

Run the demo with:

```bash
./pid_demo
```

### 2.3 Adjustment (Experimentation)

Experiment by adjusting the initial measurement, max simulation time, and setpoint in **pid_test.c** and any of the constants in **pid_test_constants.h** and recompile.