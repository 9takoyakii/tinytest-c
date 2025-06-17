//
// tinytest.c - tinytest-c is a lightweight, minimal, and simple testing framework
//              for the C programming language.
//
// Repo: https://github.com/9takoyakii/tinytest-c
// Version: 3a192212cb2ffbcb39088acfafe3896be2e386b4
// SPDX-License-Identifier: WTFPL
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tinytest.h"

#define NOT_INITIALIZED_ERROR_MESSAGE "Run the ttest_init/INIT function first"
#define ERROR(msg) printf(">> ERROR @ " msg "\n"); \
    exit(1)
#define TEST_SUITE_MAX_DEPTH 4

static ttest_cleanFunc *currCleanFunc = NULL;
static int initialized = 0;
static int concluded = 0;
static int totalPass;
static int totalFail;
static int totalSkip;
static long int totalDuration;
static struct ttest_TestSuiteStack testSuiteStack = {
    .cap = TEST_SUITE_MAX_DEPTH,
    .len = 0,
    .ptr = NULL,
};

static void updateStat(int pass, int fail, int skip) {
    totalPass += pass;
    totalFail += fail;
    totalSkip += skip;

    for (size_t i = testSuiteStack.len; i != 0; i--) {
        testSuiteStack.ptr[i - 1].totalPass += pass;
        testSuiteStack.ptr[i - 1].totalFail += fail;
        testSuiteStack.ptr[i - 1].totalSkip += skip;
    }
}

void ttest_init() {
    if (initialized) {
        ERROR("Don't call the ttest_init / INIT twice");
    }

    initialized = 1;
    testSuiteStack.len = 0;
    testSuiteStack.ptr = calloc(TEST_SUITE_MAX_DEPTH, sizeof(struct ttest_TestSuite));
}

void ttest_clean(ttest_cleanFunc *cleanFunc) {
    if (currCleanFunc != NULL) {
        ERROR("Don't call the ttest_clean / CLEAN twice");
    }

    currCleanFunc = cleanFunc;
}

void ttest_beginTestSuite(const char *desc, int skip) {
    if (!initialized) {
        ERROR(NOT_INITIALIZED_ERROR_MESSAGE);
    }

    if (testSuiteStack.len >= testSuiteStack.cap) {
        ERROR("Your test suite exceeds the allowed depth level, the maximum depth is 4");
    }

    if (testSuiteStack.len == 0) {
        if (skip) {
            printf("==== %s # SKIP\n", desc);
            return;
        } else {
            printf("==== %s\n", desc);
        }
    } else {
        if ((testSuiteStack.ptr - 1)->test.status != ttest_NONE) {
            ERROR("There are still tests running, unable to create a new test suite");
        }

        if (skip) {
            printf("==== %s >> %s # SKIP\n", (testSuiteStack.ptr - 1)->desc, desc);
            return;
        } else {
            printf("==== %s >> %s\n", (testSuiteStack.ptr - 1)->desc, desc);
        }
    }

    testSuiteStack.ptr->desc = desc;
    testSuiteStack.ptr->totalSkip = 0;
    testSuiteStack.ptr->totalFail = 0;
    testSuiteStack.ptr->totalPass = 0;
    testSuiteStack.ptr->test.status = ttest_NONE;
    testSuiteStack.ptr->totalDuration = 0;
    testSuiteStack.ptr++;
    testSuiteStack.len++;
}

void ttest_endTestSuite() {
    if (!initialized) {
        ERROR(NOT_INITIALIZED_ERROR_MESSAGE);
    }

    if (testSuiteStack.len == 0) {
        ERROR("No test suite running");
    }


    struct ttest_TestSuite *currTestSuite = testSuiteStack.ptr - 1;
    if (currTestSuite->test.status != ttest_NONE) {
        ERROR("There are still tests running, unable to cease current test suite");
    }

    if (testSuiteStack.len == 1) {
        printf("\n%s @ %d pass, %d fail, %d skip ... DONE (%lums)\n\n",
            currTestSuite->desc,
            currTestSuite->totalPass,
            currTestSuite->totalFail,
            currTestSuite->totalSkip,
            currTestSuite->totalDuration
        );

        totalPass += currTestSuite->totalPass;
        totalFail += currTestSuite->totalFail;
        totalSkip += currTestSuite->totalSkip;
        totalDuration += currTestSuite->totalDuration;
    } else {
        struct ttest_TestSuite *parentTestSuite = testSuiteStack.ptr - 2;
        parentTestSuite->totalPass += currTestSuite->totalPass;
        parentTestSuite->totalFail += currTestSuite->totalFail;
        parentTestSuite->totalSkip += currTestSuite->totalSkip;
        parentTestSuite->totalDuration += currTestSuite->totalDuration;

        printf("----- %s @ DONE (%lums)\n",
            currTestSuite->desc,
            currTestSuite->totalDuration
        );
    }

    testSuiteStack.ptr--;
    testSuiteStack.len--;
}

void ttest_beginTest(const char *desc, int failAsPassFlag, int skip) {
    if (!initialized) {
        ERROR(NOT_INITIALIZED_ERROR_MESSAGE);
    }

    if (testSuiteStack.len == 0) {
        ERROR("No test suite running");
    }

    struct ttest_TestSuite *currTestSuite = testSuiteStack.ptr - 1;
    if (currTestSuite->test.status != ttest_NONE) {
        ERROR("There is a test running, can only run 1 test at a time");
    }

    if (skip) {
        printf("----- %s >>> %s\n      # SKIP (0ms)\n",
            currTestSuite->desc,
            desc
        );
        currTestSuite->totalSkip++;
        return;
    }

    currTestSuite->test.desc = desc;
    currTestSuite->test.failAsPassFlag = failAsPassFlag;
    currTestSuite->test.status = ttest_PASS;
    currTestSuite->test.startClock = clock();
    printf("----- %s >>> %s\n",
        currTestSuite->desc,
        desc
    );
}

void ttest_endTest() {
    if (!initialized) {
        ERROR(NOT_INITIALIZED_ERROR_MESSAGE);
    }

    if (testSuiteStack.len == 0) {
        ERROR("No test suite running");
    }

    struct ttest_TestSuite *currTestSuite = testSuiteStack.ptr - 1;
    if (currTestSuite->test.status == ttest_NONE) {
        ERROR("No test is running, call ttest_endTest if there is a test running");
    }

    if (currCleanFunc != NULL) {
        currCleanFunc(&currTestSuite->test);
    }

    enum ttest_TEST_STATUS status = currTestSuite->test.status;
    long int duration = clock() - currTestSuite->test.startClock;
    currTestSuite->totalDuration += duration;
    currTestSuite->test.status = ttest_NONE;

    switch (status) {
        case ttest_PASS:
            printf("      # PASS (%lums)\n", duration);
            currTestSuite->totalPass++;
            break;
        default:
            printf("      # FAIL (%lums)\n", duration);
            currTestSuite->totalFail++;
    }
}

void ttest_conclude() {
    if (!initialized) {
        ERROR(NOT_INITIALIZED_ERROR_MESSAGE);
    }

    if (concluded) {
        ERROR("Don't call the ttest_conclude / CONCLUDE twice");
    }

    if (testSuiteStack.len != 0) {
        ERROR("There is still a test suite running, call the ttest_conclude / CONCLUDE function only when all test suites have finished");
        return;
    }

    long int duration = totalDuration;
    printf("--- %d pass, %d fail, %d skip (%lums) ---\n\n",
        totalPass,
        totalFail,
        totalSkip,
        duration
    );

    concluded = 1;
    free(testSuiteStack.ptr);
}

int ttest_assert(int expr) {
    if (!initialized) {
        ERROR(NOT_INITIALIZED_ERROR_MESSAGE);
    }

    if (testSuiteStack.len == 0) {
        ERROR("No test suite running");
    }

    struct ttest_TestSuite *currTestSuite = testSuiteStack.ptr - 1;
    if (currTestSuite->test.status == ttest_NONE) {
        ERROR("No test is running, call ttest_assert / ASSERT if there is a test running");
    }

    currTestSuite->test.status = expr ^ currTestSuite->test.failAsPassFlag ? ttest_PASS : ttest_FAIL;
    return expr;
}
