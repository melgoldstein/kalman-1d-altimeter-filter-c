#ifndef ALTIMETER_KALMAN_H
#define ALTIMETER_KALMAN_H
typedef struct { double altitude, velocity, p00, p01, p11;
    double innovation, innovation_variance; } AltimeterState;
int altimeter_step(AltimeterState *state, double dt, double acceleration,
                   double altitude, double acceleration_variance,
                   double barometer_variance);
#endif
