#include <math.h>
#include "ParamLib.h"

struct ParamLibPlatform paramLibPlatform;

void InitParamLib(struct ParamLibPlatform *platform) {
    paramLibPlatform = *platform;
}

bool ApproxEqual(float a, float b, float epsilon) {
    return fabsf(a - b) < epsilon;
}
