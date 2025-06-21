What is tinytest-c?
----------------------------
Tinytest-c is a tiny and simple testing framework for the C programming language.

It is designed to be easy to use, with no external dependencies, and has a very small memory and code footprint — making it ideal for embedded systems or small C projects.


Functions & Macros:
----------------------------

** void ttest_init(void):
    Initializes the testing framework. Must be called once before using any other testing functions.

** INIT():
    Macro alias for ttest_init().

** void ttest_cleanUp(void func(const struct ttest_Test *test), int onlyForThis):
    Registers a cleanup function that will always be run after a test completes. If onlyForThis = 1, the cleanup function will only run for tests inside the current describe block. If called outside of a describe, the onlyForThis flag is ignored.

** CLEANUP(func):
    Macro alias for ttest_cleanUp(func, 0).

** CLEANUP_THIS(func):
    Macro alias for ttest_cleanUp(func, 1).

** void ttest_beginTestSuite(const char *desc, int skip):
    Starts a test suite with a description. If skip = 1, the suite is marked as skipped, but the code block will still be executed.

** void ttest_endTestSuite(void):
    Ends the current test suite.

** DESCRIBE_BEGIN(desc):
    Macro to begin a test suite. Internally calls ttest_beginTestSuite(desc, 0).

** DESCRIBE_SKIP(desc):
    Macro to create a skipped test suite. Internally calls ttest_beginTestSuite(desc, 1) and prevents execution of the block.

** DESCRIBE_END():
    Macro to end a test suite. Internally calls ttest_endTestSuite().

** void ttest_beginTest(const char *desc, int failAsPassFlag, int skip):
    Begins a test case with a description. If failAsPassFlag = 1, fail will be considered a pass (and vice versa). If skip = 1, the test will be marked as skipped and the test will not run (cleanup function also will not run).

** void ttest_endTest(void):
    Ends the current test case.

** IT_BEGIN(desc):
    Macro to begin a normal test case. Internally calls ttest_beginTest(desc, 0, 0).

** IT_SKIP_BEGIN(desc):
    Macro to begin a skipped test case. Internally calls ttest_beginTest(desc, 0, 1).

** IT_FAIL_BEGIN(desc):
    Macro to begin a test case where fail is treated as pass. Internally calls ttest_beginTest(desc, 1, 0).

** IT_END(desc):
    Macro to end a test case. Internally calls ttest_endTest().

** void ttest_test(const char *desc, int failAsPassFlag, int skip, ttest_testFunc test):
    Runs a test function with specified flags for forced pass/fail or skip behavior.

** IT(desc, func):
    Macro to define and run a normal test function.

** IT_SKIP(desc, func):
    Macro to define and run a skipped test function. The test will be registered as skipped and will not be executed.

** IT_FAIL(desc, func):
    Macro to define and run a test function where fail is considered pass and pass is considered fail.

** int ttest_conclude(void):
    Finalizes the test run, prints a summary of all results, and returns an exit code. Returns 1 if any test failed, 0 if all passed.

** CONCLUDE():
    Macro alias for ttest_conclude().

** int ttest_assert(int expr):
    Evaluates the expression. If false (i.e., 0), the test is marked as failed, but execution continues.

** ASSERT(expr):
    Macro alias for ttest_assert(expr).

** ASSERT_N(expr):
    Asserts that the expression is false. Internally evaluates !expr.


Example:
----------------------------
#include <stdio.h>
#include "tinytest.h"

void globalCleanup(const struct ttest_Test *test) {
    printf("Cleaning up after test: %s\n", test->desc);
}

int test_addition(const struct ttest_Test *test) {
    int result = 2 + 2;
    ASSERT(result == 4);
    return 0;
}

int test_subtraction(const struct ttest_Test *test) {
    int result = 10 - 3;
    ASSERT(result == 7);
    ASSERT(result != 6);
    return 0;
}

int test_fail(const struct ttest_Test *test) {
    int x = 5;
    ASSERT(x == 10);
    return 0;
}

int test_skipped(const struct ttest_Test *test) {
    ASSERT(1 == 0);
    return 0;
}

int main(void) {
    INIT();
    CLEANUP(&globalCleanup);

    DESCRIBE_BEGIN("Math Operations");
        IT("Addition works", &test_addition);
        IT("Subtraction works", &test_subtraction);
    DESCRIBE_END();

    DESCRIBE_BEGIN("Edge Cases");
        IT_FAIL("Failure is expected", &test_fail);
        IT_SKIP("Skipped test", &test_skipped);
    DESCRIBE_END();

    return CONCLUDE();
}

License:
----------------------------
Tinytest-c is licensed under the MIT License.
