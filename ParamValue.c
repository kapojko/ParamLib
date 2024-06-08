#include <math.h>
#include "MinUnit.h"
#include "ParamValue.h"

void InitParamValue(struct ParamValue *pv, int maxDataAge, short averageCount) {
    pv->set = false;
    pv->value = 0.0;
    pv->valueBufIndex = 0;
    pv->valueBufCount = 0;
    pv->maxDataAge = maxDataAge;
    pv->averageCount = (averageCount <= MAX_AVERAGE_COUNT) ? averageCount : MAX_AVERAGE_COUNT;
}

void UpdateParamValue(struct ParamValue *pv, float value, ParamTime ctime) {
    // update value
    if (pv->averageCount == 0) {
        // no averaging
        pv->value = value;
    } else {
        // add to buffer
        pv->valueBuf[pv->valueBufIndex] = value;

        // move index
        pv->valueBufIndex = (pv->valueBufIndex + 1) % MAX_AVERAGE_COUNT;
        if (pv->valueBufCount < pv->averageCount) {
            pv->valueBufCount++;
        }

        // average value in buffer
        float value = 0.0;
        for (short i = 0; i < pv->valueBufCount; i++) {
            short bufIndex = (pv->valueBufIndex - pv->valueBufCount + i + MAX_AVERAGE_COUNT) % MAX_AVERAGE_COUNT;
            value += pv->valueBuf[bufIndex];
        }
        pv->value = value / pv->valueBufCount;
    }

    // update time
    pv->updateTime = ctime;

    // set flag
    pv->set = true;
}

void ParamValueStep(struct ParamValue *pv, ParamTime ctime) {
    // check data age
    if (pv->maxDataAge != 0) {
        int age = paramLibPlatform.getTimeDiffMs(ctime, pv->updateTime);
        if (age > pv->maxDataAge) {
            ClearParamValue(pv);
        }
    }
}

void ClearParamValue(struct ParamValue *pv) {
    pv->set = false;
    pv->value = 0.0;
    pv->valueBufIndex = 0;
    pv->valueBufCount = 0;
}

const char *ParamValue_UnitTest(void) {
    struct ParamValue pv;

    // Test plain value
    InitParamValue(&pv, 0, 0);
    mu_assert("Value is not set", !pv.set);
    UpdateParamValue(&pv, 1.0f, 1000);
    mu_assert("Value should be 1.0 and set", pv.value == 1.0f && pv.set);
    mu_assert("Value update time should be 1000", pv.updateTime == 1000);

    // Test average value
    InitParamValue(&pv, 0, 2);
    UpdateParamValue(&pv, 1.0f, 1000);
    mu_assert("Value should be 1.0, count should be 1, index should be 1", ApproxEqual(pv.value, 1.0f, 0.01f) && pv.valueBufCount == 1 && pv.valueBufIndex == 1);
    UpdateParamValue(&pv, 2.0f, 2000);
    mu_assert("Value should be 1.5, count should be 2, index should be 2", ApproxEqual(pv.value, 1.5f, 0.01f) && pv.valueBufCount == 2 && pv.valueBufIndex == 2);
    UpdateParamValue(&pv, 3.0f, 3000);
    mu_assert("Value should be 2.5, count should be 2, index should be 0", ApproxEqual(pv.value, 2.5f, 0.01f) && pv.valueBufCount == 2 && pv.valueBufIndex == 0);

    // Test max age
    InitParamValue(&pv, 1500, 0);
    UpdateParamValue(&pv, 1.0f, paramLibPlatform.freq);
    mu_assert("Value should be 1.0f, update time should be 1 sec", pv.value == 1.0f && pv.updateTime == paramLibPlatform.freq);
    ParamValueStep(&pv, 2 * paramLibPlatform.freq);
    mu_assert("Value should be still set", pv.set);
    ParamValueStep(&pv, 4 * paramLibPlatform.freq);
    mu_assert("Value should be cleared", !pv.set);

    // Test clear
    InitParamValue(&pv, 0, 0);
    UpdateParamValue(&pv, 1.0f, 1000);
    ClearParamValue(&pv);
    mu_assert("Value should be cleared", !pv.set);

    return 0;
}
