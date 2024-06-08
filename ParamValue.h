#ifndef PARAMVALUE_H
#define PARAMVALUE_H

#include <stdbool.h>
#include "ParamLib.h"

#define MAX_AVERAGE_COUNT 3

struct ParamValue {
    bool set;
    float value;
    
    float valueBuf[MAX_AVERAGE_COUNT];
    short valueBufIndex;
    short valueBufCount;
    ParamTime updateTime;
    
    int maxDataAge;
    short averageCount;
};

// Init ParamValue, maxDataAge: max data age in ms (or 0 if no limit), averageCount: number of values to average (or 0 if no average)
void InitParamValue(struct ParamValue *pv, int maxDataAge, short averageCount);
void UpdateParamValue(struct ParamValue *pv, float value, ParamTime ctime);
void ParamValueStep(struct ParamValue *pv, ParamTime ctime);
void ClearParamValue(struct ParamValue *pv);

const char *ParamValue_UnitTest(void);

#endif // PARAMVALUE_H
