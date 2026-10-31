# One-axis altimeter filter

[Portfolio: melgoldstein.com](https://melgoldstein.com/)

Constant-acceleration prediction with a two-state altitude/velocity Kalman filter.
Acceleration is an external vertical input with gravity already removed. Barometer
updates use a scalar Joseph covariance correction; rejected inputs leave state intact.

```sh
make
make test
./altimeter data/baro_noisy.csv > estimates.csv
```

| Output column | Meaning |
|---|---|
| altitude_m | Posterior vertical position |
| velocity_mps | Posterior vertical speed |
| altitude_variance | Posterior uncertainty, m squared |
| innovation_m | Measured minus predicted altitude |
| innovation_variance | Innovation uncertainty, m squared |

The CSV fixture follows h=t squared with seeded Gaussian barometer and acceleration
noise. CLI parameters use acceleration variance 0.25 and barometer variance 25;
library callers supply their own sensor variances and positive sampling intervals.
No attitude, accelerometer bias or pressure-to-altitude correction is estimated.
The test checks an analytic trajectory, covariance positivity and rejection atomicity.
MIT; C99 compiler and libm only.

## Library integration

```c
#include "kalman.h"
AltimeterState estimate = {0, 0, 100, 0, 100, 0, 0};
int accepted = altimeter_step(&estimate, 0.1, 2.0, 0.03, 0.25, 25.0);
```

The initial covariance is in squared meters and squared meters/second. `p01`
is the position/velocity cross covariance. `altimeter_step` returns zero on
invalid inputs without changing the estimate. Choose initial uncertainty to match
the launch reference and speed estimate; the CLI starts at altitude and speed zero.
Time stamps must increase strictly and all inputs must be finite.
