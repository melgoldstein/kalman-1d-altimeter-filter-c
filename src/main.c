#include "kalman.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    if (argc != 2) { fprintf(stderr, "usage: altimeter readings.csv\n"); return 2; }
    FILE *input = fopen(argv[1], "r");
    if (!input) { perror(argv[1]); return 1; }
    char line[512], extra;
    AltimeterState state = {0, 0, 100, 0, 100, 0, 0};
    double previous = 0;
    unsigned rows = 0;
    if (!fgets(line, sizeof line, input)) { fclose(input); return 1; }
    puts("time_s,altitude_m,velocity_mps,altitude_variance,innovation_m,innovation_variance");
    while (fgets(line, sizeof line, input)) {
        double timestamp, acceleration, altitude;
        if (sscanf(line, "%lf,%lf,%lf %c", &timestamp, &acceleration, &altitude, &extra) != 3 ||
            !altimeter_step(&state, timestamp-previous, acceleration, altitude, 0.25, 25)) {
            fprintf(stderr, "invalid measurement at row %u\n", rows+2);
            fclose(input); return 1;
        }
        printf("%.3f,%.6f,%.6f,%.6f,%.6f,%.6f\n", timestamp, state.altitude,
            state.velocity, state.p00, state.innovation, state.innovation_variance);
        previous = timestamp;
        ++rows;
    }
    int failed = ferror(input) || !rows;
    fclose(input);
    return failed;
}
