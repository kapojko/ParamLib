#ifndef PARAMAVERAGE_H
#define PARAMAVERAGE_H

#include <stdbool.h>
#include "ParamLib.h"

#define MAX_AVERAGE_COUNT 3

struct ParamAverage {
    bool set;
    float value;
    
    float valueBuf[MAX_AVERAGE_COUNT];
    short valueBufIndex;
    short valueBufCount;
    ParamTime updateTime;
    
    int maxDataAge;
    short averageCount;
};

// Init ParamAverage, maxDataAge: max data age in ms (or 0 if no limit), averageCount: number of values to average (or 0 if no average)
void InitParamAverage(struct ParamAverage *pv, int maxDataAge, short averageCount);
void UpdateParamAverage(struct ParamAverage *pv, float value, ParamTime ctime);
void ParamAverageStep(struct ParamAverage *pv, ParamTime ctime);
void ClearParamAverage(struct ParamAverage *pv);

const char *ParamAverage_UnitTest(void);

#endif // PARAMAVERAGE_H
