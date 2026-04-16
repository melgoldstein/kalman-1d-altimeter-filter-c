#include "kalman.h"
#include <assert.h>
#include <math.h>
#include <string.h>
int main(void) {
    AltimeterState state = {0,0,100,0,100,0,0};
    for (int index=1; index<=1000; ++index) {
        double timestamp=index*0.1;
        assert(altimeter_step(&state,0.1,2,timestamp*timestamp,0.01,1));
        assert(state.p00>=0 && state.p11>=0);
        assert(state.p00*state.p11-state.p01*state.p01>=-1e-10);
    }
    assert(fabs(state.altitude-10000)<0.01);
    assert(fabs(state.velocity-200)<0.01);
    AltimeterState saved=state;
    assert(!altimeter_step(&state,-1,0,0,1,1));
    assert(!memcmp(&saved,&state,sizeof state));
    assert(!altimeter_step(&state,0.1,NAN,0,1,1));
    assert(!altimeter_step(&state,0.1,0,0,1,0));
    return 0;
}
