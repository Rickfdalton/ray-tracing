#ifndef RANDOM_H_
#define RANDOM_H_

#include <cstdint>
#include <omp.h>

struct Xoshiro256PlusPlus {
    uint64_t s[4];

    Xoshiro256PlusPlus(uint64_t seed) {
        // simple splitmix64 to initialize state
        uint64_t x = seed;
        for(int i=0;i<4;i++){
            x += 0x9e3779b97f4a7c15;
            uint64_t z = x;
            z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
            z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
            s[i] = z ^ (z >> 31);
        }
    }

    inline uint64_t rotl(const uint64_t x, int k) {
        return (x << k) | (x >> (64 - k));
    }

    inline uint64_t next() {
        uint64_t result = rotl(s[0] + s[3], 23) + s[0];

        uint64_t t = s[1] << 17;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;

        s[3] = rotl(s[3], 45);

        return result;
    }

    inline float next_float() {
        return (next() >> 11) * (1.0f / 9007199254740992.0f); // divide by 2^53
    }
};

// thread-local RNG
inline float random_float() {
    thread_local Xoshiro256PlusPlus rng(
        1234 + 
#ifdef _OPENMP
        omp_get_thread_num() * 9973
#else
        0
#endif
    );
    return rng.next_float();
}

#endif