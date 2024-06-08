#ifndef PARAMVALUE_H
#define PARAMVALUE_H

#include <stdbool.h>
#include "ParamLib.h"

struct ParamValue {
    bool set;
    float value;
    float alpha;
    ParamTime updateTime;
    int maxDataAge;
};

void InitParamValue(struct ParamValue *pv, int maxDataAge, float alpha);
void InitParamValueForN(struct ParamValue *pv, int maxDataAge, int n);
void UpdateParamValue(struct ParamValue *pv, float value, ParamTime ctime);
void ParamValueStep(struct ParamValue *pv, ParamTime ctime);
void ClearParamValue(struct ParamValue *pv);

const char *ParamValue_UnitTest(void);

#endif // PARAMVALUE_H
