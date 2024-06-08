#ifdef TESTING

#include <stdio.h>
#include <time.h>
#include "MinUnit.h"
#include "ParamLib.h"
#include "ParamState.h"
#include "ParamValue.h"

int tests_run = 0;

static const char *RunAllTests(void) {
    mu_run_test(ParamState_UnitTest);
    mu_run_test(ParamValue_UnitTest);
    return 0;
}

static int getTimeDiffMs(ParamTime end, ParamTime start) {
    return end - start;
}

int main(void) {
    struct ParamLibPlatform platform = {
        .getTimeDiffMs = getTimeDiffMs,
        .invalidTime = 0xFFFFFFFF,
        .freq = 1000
    };
    InitParamLib(&platform);

    const char *result = RunAllTests();
    if (result) {
        printf("TEST FAILED: %s\n", result);
        return 1;
    } else {
        printf("ALL TESTS PASSED (%d)\n", tests_run);
        return 0;
    }
}

#endif
