// random.h

#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>
#include <time.h>

static inline void rand_seed() {srand(time(NULL));}

static inline int rand_int(int min, int max) {return rand() % (max - min + 1) + min;}

#endif