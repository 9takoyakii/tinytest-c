/*
    tinytest.h - tinytest-c is a lightweight, minimal, and simple testing framework
                 for the C programming language.

    Version: 1.0
    Repository: https://github.com/9takoyakii/tinytest-c
    SPDX-License-Identifier: MIT
*/

#ifndef __TINY_TEST_C
#define __TINY_TEST_C

#include <stddef.h>

enum ttest_TEST_STATUS {
    ttest_NONE,
    ttest_PASS,
    ttest_FAIL,
    ttest_SKIP,
};

struct ttest_Test {
    enum ttest_TEST_STATUS status;
    const char *desc;
    int failAsPassFlag;
    long startClock;
};

typedef void ttest_cleanupFunc(const struct ttest_Test *test);

struct ttest_TestSuite {
    const char *desc;
    struct ttest_Test test;
    int totalPass;
    int totalFail;
    int totalSkip;
    long totalDuration;

    struct {
        ttest_cleanupFunc *cleanupFunc;
        int onlyForThis;
    } cleanup;
};

struct ttest_TestSuiteStack {
    struct ttest_TestSuite *nextPtr;
    size_t cap;
    size_t len;
};

void ttest_init(void);
#define INIT() ttest_init()

void ttest_cleanUp(ttest_cleanupFunc cleanFunc, int thisOnly);
#define CLEANUP(func) ttest_cleanUp(&func, 0)
#define CLEANUP_THIS(func) ttest_cleanUp(&func, 1);

void ttest_beginTestSuite(const char *desc, int skip);
void ttest_endTestSuite(void);
#define DESCRIBE(desc, block) ttest_beginTestSuite(desc, 0); \
    { block }; \
    ttest_endTestSuite()
#define DESCRIBE_SKIP(desc, block) ttest_beginTestSuite(desc, 1)

void ttest_beginTest(const char *desc, int failAsPassFlag, int skip);
void ttest_endTest(void);
#define IT(desc, block) ttest_beginTest(desc, 0, 0); \
    { block }; \
    ttest_endTest()
#define IT_SKIP(desc, block) ttest_beginTest(desc, 0, 1)
#define IT_FAIL(desc, block) ttest_beginTest(desc, 1, 0); \
    { block }; \
    ttest_endTest()

int ttest_conclude(void);
#define CONCLUDE() ttest_conclude();

int ttest_assert(int expr);
#define ASSERT(expr) ttest_assert(expr);

#endif
