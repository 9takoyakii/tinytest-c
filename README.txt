@@ What is tinytest-c
tinytest-c is a lightweight, minimal, and simple testing framework for the C programming language.

It is designed to be easy to use, with no external dependencies, and has a very small memory and code footprint — making it ideal for embedded systems or small C projects.


@@ Functions and Macros
--- void ttest_init()
Initializes the testing framework. Must be called once before any other functions.

--- INIT()
Macro alias for ttest_init().

--- void ttest_cleanUp(void func(const struct ttest_Test *test), int onlyForThis)
Registers a cleanup function that will always be run after the test is complete. If onlyForThis = 1, then the cleanup function will only be run in the describe where the cleanUp function is registered (if outside the describe block, then this will be ignored).

--- CLEANUP(func)
Macro alias for ttest_clean(func, 0).

--- CLEANUP_THIS(func)
Macro alias for ttest_clean(func, 1).

--- void ttest_beginTestSuite(const char *desc, int skip)
Starts an individual test with a description. If failAsPassFlag = 1, then a failed test will be considered successful and vice versa. If skip = 1, the test will be marked as skipped, but the next instruction will still be executed.

--- void ttest_endTestSuite()
Ends the current test suite.

--- DESCRIBE(desc, block)
Macro to simplify test suite creation. Calls ttest_beginTestSuite(), runs the block, and then calls ttest_endTestSuite().

--- DESCRIBE_SKIP(desc, block)
Creates a skipped test suite. The block will not be executed.

--- void ttest_beginTest(const char *desc, int failAsPassFlag, int skip)
Begins a test case with a description. Allows flags to force pass or skip.

--- void ttest_endTest()
Ends the current test.

--- IT(desc, block)
Macro to define a test case.

--- IT_SKIP(desc, block)
Defines a test case that will be skipped. The block will not be executed.

--- IT_FAIL(desc, block)
Defines a test case where if it fails it will be considered successful and vice versa.

--- int ttest_conclude()
Prints the test results summary. Must be called once at the end. The function also returns an exit code, 1 if any one failed and 0 if all passed.

--- CONCLUDE()
Macro alias for ttest_conclude().

--- int ttest_assert(int expr)
Evaluates the expression. If false, the test fails but execution keep continues.

--- ASSERT(expr)
Macro alias for ttest_assert(expr).


@@ Example
```c
#include "math.h"
#include "tinytest.h"

int main() {
    INIT();
    DESCRIBE("Testing file math.c", {
        IT("Testing 2 ** 5", {
            ASSERT(pow(2, 5) == 32);
        });

        IT("Testing log2(32) + sqrt(100)", {
            ASSERT(log2(32) + sqrt(100) == 15);
        });
    });
    return CONCLUDE();
}
``

@@ License
Tinytest-c is licensed under the MIT License.
