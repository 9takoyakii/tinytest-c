#ifndef __TINY_TEST_C
#define __TINY_TEST_C

#include <stddef.h>

// NOTE: Tests that are skipped are processed directly
//       so test information is not stored in the test suite stack.
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

struct ttest_TestSuite {
    const char *desc;
    struct ttest_Test test;
    int totalPass;
    int totalFail;
    int totalSkip;
    long totalDuration;
};

struct ttest_TestSuiteStack {
    struct ttest_TestSuite *ptr;
    size_t cap;
    size_t len;
};

typedef void ttest_cleanFunc(const struct ttest_Test *test);

void ttest_init();
#define INIT() ttest_init()

void ttest_clean(ttest_cleanFunc cleanFunc);
#define CLEAN(func) ttest_clean(&func)

void ttest_beginTestSuite(const char *desc, int skip);
void ttest_endTestSuite();
#define DESCRIBE(desc, block) ttest_beginTestSuite(desc, 0); \
    { block }; \
    ttest_endTestSuite()
#define DESCRIBE_SKIP(desc, block) ttest_beginTestSuite(desc, 1)

void ttest_beginTest(const char *desc, int failAsPassFlag, int skip);
void ttest_endTest();
#define IT(desc, block) ttest_beginTest(desc, 0, 0); \
    { block }; \
    ttest_endTest()
#define IT_SKIP(desc, block) ttest_beginTest(desc, 0, 1)
#define IT_FAIL(desc, block) ttest_beginTest(desc, 1, 0); \
    { block }; \
    ttest_endTest()

void ttest_conclude();
#define CONCLUDE() ttest_conclude();

void ttest_assert(int expr);
#define ASSERT(expr) ttest_assert(expr);


#endif
