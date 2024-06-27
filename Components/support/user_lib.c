#include "user_lib.h"

void ramp_int32_init(ramp_int32_t *ramp, int32_t init_value, int32_t input_min, int32_t input_max, int32_t value_min, int32_t value_max)
{
    ramp->input = input_min;
    ramp->input_min = input_min;
    ramp->input_max = input_max;
    ramp->value_min = value_min;
    ramp->value_max = value_max;
    ramp->value = init_value;
}

int32_t ramp_int32_calc(ramp_int32_t *ramp, int32_t input)
{
    if (input < ramp->input_min) {
        ramp->input = ramp->input_min;
    } else if (input > ramp->input_max) {
        ramp->input = ramp->input_max;
    } else {
        ramp->input = input;
    }

    int32_t value = ramp->value + ramp->input;
    if (value < ramp->value_min) {
        ramp->value = ramp->value_min;
    } else if (value > ramp->value_max) {
        ramp->value = ramp->value_max;
    } else {
        ramp->value = value;
    }

    return ramp->value;
}

void ramp_int32_update(ramp_int32_t *ramp, int32_t set)
{
    if (set < ramp->value_min) {
        ramp->value = ramp->value_min;
    } else if (set > ramp->value_max) {
        ramp->value = ramp->value_max;
    } else {
        if (set - ramp->value < ramp->input_min) {
            ramp->value += ramp->input_min; 
        } else if (set - ramp->value > ramp->input_max) {
            ramp->value += ramp->input_max;
        } else {
            ramp->value = set;
        }
    }
}
