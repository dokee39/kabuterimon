#ifndef _USER_LIB_H_
#define _USER_LIB_H_

#include <stdint.h>

#define ABS(x)       ((x) < 0 ? -(x) : (x))

typedef struct {
    int32_t input;
    int32_t input_min;
    int32_t input_max;
    int32_t value;
    int32_t value_min;
    int32_t value_max;
} ramp_int32_t;

void ramp_int32_init(ramp_int32_t *ramp, int32_t init_value, int32_t input_min, int32_t input_max, int32_t value_min, int32_t value_max);
int32_t ramp_int32_calc(ramp_int32_t *ramp, int32_t input);

#endif

