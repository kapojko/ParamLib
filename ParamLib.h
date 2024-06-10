#ifndef PARAMLIB_H
#define PARAMLIB_H

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t ParamTime;
typedef int32_t ParamTimeDiff;

struct ParamLibPlatform {
    ParamTimeDiff (*getTimeDiffMs)(ParamTime end, ParamTime start);
    ParamTime invalidTime;
    ParamTime freq;
};

extern struct ParamLibPlatform paramLibPlatform;

void InitParamLib(struct ParamLibPlatform *platform);

bool ApproxEqual(float a, float b, float epsilon);

#endif // PARAMLIB_H
