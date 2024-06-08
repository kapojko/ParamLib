#ifndef PARAMSTATE_H
#define PARAMSTATE_H

#include <stdbool.h>
#include "ParamLib.h"

struct ParamState {
    int value;

    int changeAckCount;
    int changeAckCur;

    ParamTime updateTime;

    int resetValue;
    int resetPeriod;
};

void InitParamState(struct ParamState *state, int value, int changeAckCount, int resetPeriod);
bool UpdateParamState(struct ParamState *state, int value, ParamTime ctime);
bool ParamStateStep(struct ParamState *state, ParamTime ctime);
void ResetParamState(struct ParamState *state);

const char *ParamState_UnitTest(void);

#endif // PARAMSTATE_H
