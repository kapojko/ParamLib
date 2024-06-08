#include "MinUnit.h"
#include "ParamState.h"

void InitParamState(struct ParamState *state, int value, int changeAckCount, int resetPeriod) {
    state->value = value;
    state->changeAckCount = changeAckCount;
    state->changeAckCur = 0;
    state->updateTime = paramLibPlatform.invalidTime;
    state->resetValue = value;
    state->resetPeriod = resetPeriod;
}

bool UpdateParamState(struct ParamState *state, int value, ParamTime ctime) {
    if (state->value != value) {
        state->changeAckCur++;

        if (state->changeAckCur > state->changeAckCount) {
            state->value = value;
            state->changeAckCur = 0;
            state->updateTime = ctime;
            return true;
        }
    } else {
        state->changeAckCur = 0;
        state->updateTime = ctime;
    }

    return false;
}

bool ParamStateStep(struct ParamState *state, ParamTime ctime) {
    if (state->resetPeriod != 0 && state->updateTime != paramLibPlatform.invalidTime &&
        paramLibPlatform.getTimeDiffMs(ctime, state->updateTime) > state->resetPeriod) {
        ResetParamState(state);
        return true;
    }

    return false;
}

void ResetParamState(struct ParamState *state) {
    state->value = state->resetValue;
    state->changeAckCur = 0;
    state->updateTime = paramLibPlatform.invalidTime;
}

const char *ParamState_UnitTest(void) {
    struct ParamState state;

    // Test update without ack
    InitParamState(&state, 0, 0, 0);
    mu_assert("Value should be 0 and not changed", !UpdateParamState(&state, 0, 0) && state.value == 0);
    mu_assert("Value should be 1 and changed", UpdateParamState(&state, 1, 1000) && state.value == 1);
    mu_assert("Value should be 2 and changed", UpdateParamState(&state, 2, 2000) && state.value == 2);

    // Test with ack
    InitParamState(&state, 0, 2, 0);
    mu_assert("Value should be 0 and not changed", !UpdateParamState(&state, 1, 0) && state.value == 0);
    mu_assert("Value should be 0 and not changed", !UpdateParamState(&state, 1, 1000) && state.value == 0);
    mu_assert("Value should be 1 and changed", UpdateParamState(&state, 1, 2000) && state.value == 1);

    // Test timed reset
    InitParamState(&state, 10, 0, 2000);
    mu_assert("Value should be 20 and changed", UpdateParamState(&state, 20, 0) && state.value == 20);
    mu_assert("Value should be still 20 (not changed)", !ParamStateStep(&state, 1 * paramLibPlatform.freq) && state.value == 20);
    mu_assert("Value should be reset to 10 (changed)", ParamStateStep(&state, 3 * paramLibPlatform.freq) && state.value == 10);
    mu_assert("Value should be 50 and changed", UpdateParamState(&state, 50, 10 * paramLibPlatform.freq) && state.value == 50);

    return 0;
}
