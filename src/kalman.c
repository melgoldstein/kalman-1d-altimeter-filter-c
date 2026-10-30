#include "kalman.h"
#include <math.h>
int altimeter_step(AltimeterState *state, double dt, double acceleration,
                   double altitude, double acceleration_variance,
                   double barometer_variance) {
    if (!state || !isfinite(dt) || dt <= 0 || !isfinite(acceleration) ||
        !isfinite(altitude) || !isfinite(acceleration_variance) ||
        acceleration_variance < 0 || !isfinite(barometer_variance) ||
        barometer_variance <= 0 || !isfinite(state->altitude) ||
        !isfinite(state->velocity) || !isfinite(state->p00) ||
        !isfinite(state->p01) || !isfinite(state->p11) || state->p00 < 0 ||
        state->p11 < 0 || state->p00 * state->p11 < state->p01 * state->p01)
        return 0;
    AltimeterState next = *state;
    double dt2 = dt * dt, q = acceleration_variance;
    double p00 = state->p00 + 2 * dt * state->p01 + dt2 * state->p11 + dt2 * dt2 * q / 4;
    double p01 = state->p01 + dt * state->p11 + dt2 * dt * q / 2;
    double p11 = state->p11 + dt2 * q;
    next.altitude += dt * state->velocity + 0.5 * dt2 * acceleration;
    next.velocity += dt * acceleration;
    next.innovation = altitude - next.altitude;
    next.innovation_variance = p00 + barometer_variance;
    double k0 = p00 / next.innovation_variance;
    double k1 = p01 / next.innovation_variance;
    next.altitude += k0 * next.innovation;
    next.velocity += k1 * next.innovation;
    /* Joseph form retains covariance symmetry through the scalar update. */
    next.p00 = (1-k0)*(1-k0)*p00 + k0*k0*barometer_variance;
    next.p01 = (1-k0)*(p01-k1*p00) + k0*k1*barometer_variance;
    next.p11 = p11 - 2*k1*p01 + k1*k1*(p00+barometer_variance);
    if (!isfinite(next.altitude) || !isfinite(next.velocity) ||
        !isfinite(next.p00) || !isfinite(next.p01) || !isfinite(next.p11)) return 0;
    *state = next;
    return 1;
}
