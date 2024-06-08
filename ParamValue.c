#include <math.h>
#include "MinUnit.h"
#include "ParamValue.h"

void InitParamValue(struct ParamValue *pv, int maxDataAge, float alpha) {
    pv->set = false;
    pv->value = 0.0;
    pv->alpha = alpha;
    pv->maxDataAge = maxDataAge;
}

void InitParamValueForN(struct ParamValue *pv, int maxDataAge, int n) {
    // Set alpha as an "average" of n values
    float alpha = 1.0f - 1.0f / n;

    InitParamValue(pv, maxDataAge, alpha);  
}

void UpdateParamValue(struct ParamValue *pv, float value, ParamTime ctime) {
    if (!pv->set) {
        pv->value = value;
    } else {
        pv->value = pv->alpha * value + (1.0f - pv->alpha) * pv->value;
    }
    pv->updateTime = ctime;
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
}

const char *ParamValue_UnitTest(void) {
    struct ParamValue pv;

    // Test plain value
    InitParamValue(&pv, 0, 0.5f);
    mu_assert("Value is not set", !pv.set);
    UpdateParamValue(&pv, 1.0f, 1000);
    mu_assert("Value should be 1.0 and set", pv.value == 1.0f && pv.set);
    mu_assert("Update time should be 1000", pv.updateTime == 1000);

    // Test exponential smoothing
    InitParamValue(&pv, 0, 0.5f);
    UpdateParamValue(&pv, 1.0f, 1000);
    UpdateParamValue(&pv, 2.0f, 2000);
    mu_assert("Value should be 1.5", ApproxEqual(pv.value, 1.5f, 0.01f));
    UpdateParamValue(&pv, 3.0f, 3000);
    mu_assert("Value should be 2.25", ApproxEqual(pv.value, 2.25f, 0.01f));

    // Test max age
    InitParamValue(&pv, 1500, 0.5f);
    UpdateParamValue(&pv, 1.0f, paramLibPlatform.freq);
    mu_assert("Value should be 1.0, update time should be 1 sec", pv.value == 1.0f && pv.updateTime == paramLibPlatform.freq);
    ParamValueStep(&pv, 2 * paramLibPlatform.freq);
    mu_assert("Value should be still set", pv.set);
    ParamValueStep(&pv, 4 * paramLibPlatform.freq);
    mu_assert("Value should be cleared", !pv.set);

    // Test clear
    InitParamValue(&pv, 0, 0.5f);
    UpdateParamValue(&pv, 1.0f, 1000);
    ClearParamValue(&pv);
    mu_assert("Value should be cleared", !pv.set);

    // Test InitParamValueForN
    InitParamValueForN(&pv, 0, 3);
    mu_assert("Alpha should be 0.667", ApproxEqual(pv.alpha, 0.667f, 0.01f));

    return 0;
}
