#ifndef RANDOM_H_
#define RANDOM_H_

#include <cstdlib> 

#ifdef _OPENMP
#include <omp.h>
#endif

inline float random_float() {
    thread_local unsigned short xsubi[3] = {0, 0, 0};
    thread_local bool init = false;
    if (!init) {
        int tid = 
#ifdef _OPENMP
            omp_get_thread_num();
#else
            0;
#endif
        xsubi[0] = 1234 + tid * 17;
        xsubi[1] = 5678 + tid * 31;
        xsubi[2] = 9012 + tid * 13;
        init = true;
    }
    return (float)erand48(xsubi);
}

#endif