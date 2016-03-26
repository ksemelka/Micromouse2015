#ifndef PID_H
#define PID_H

class PID {
  public:
    PID(double*, double*, double*,        // * Constructor. Links the PID to the Input, Output, and
        double, double, double);          //   Setpoint. Tuning parameters are also set here.

    bool Compute();                       // * Performs the PID calculation. It should be
                                          //   called every time loop() cycles.

    void SetOutputLimits(double, double); // clamps the output to a specific range.

    // Display functions
    double GetKp() const;			// These functions query the pid for internal values.
    double GetKi() const;
    double GetKd() const;
  private:
    void Initialize();
    void SetTunings(double, double, double);  // Used in constructor

    double kp;        // Proportional tuning value
    double ki;        // Integral tuning value
    double kd;        // Derivative tuning value

    double* myInput;              // * Pointers to the Input, Output, and Setpoint variables.
    double* myOutput;             //   This creates a hard link between the variables and the
    double* mySetpoint;           //   PID, freeing the user from having to constantly tell us
                                  //   what these values are. With pointers we'll just know.

    unsigned long lastTime;
    double ITerm, lastInput;
    double outMin, outMax;
    const double SampleTime = 100.0;
};

#endif /*PID_H*/
