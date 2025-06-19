What is tinytest-c?
----------------------------
Tinytest-c is a tiny and simple testing framework for the C programming language.

It is designed to be easy to use, with no external dependencies, and has a very small memory and code footprint — making it ideal for embedded systems or small C projects.


Functions & Macros:
----------------------------

1. void ttest_init(): Initializes the testing framework. Must be called once before any other functions.

2. INIT(): Macro alias for ttest_init().

3. void ttest_cleanUp(void func(const struct ttest_Test *test), int onlyForThis): Registers a cleanup function that will always be run after the test is complete. If onlyForThis = 1, then the cleanup function will only be run in the describe where the cleanUp function is registered (if outside the describe block, then this will be ignored).

4. CLEANUP(func): Macro alias for ttest_clean(func, 0).

5. CLEANUP_THIS(func): Macro alias for ttest_clean(func, 1).

6. void ttest_beginTestSuite(const char *desc, int skip): Starts an individual test with a description. If failAsPassFlag = 1, then a failed test will be considered successful and vice versa. If skip = 1, the test will be marked as skipped, but the next instruction will still be executed.

7. void ttest_endTestSuite(): Ends the current test suite.

8. DESCRIBE(desc, block): Macro to simplify test suite creation. Calls ttest_beginTestSuite(), runs the block, and then calls ttest_endTestSuite().

9. DESCRIBE_SKIP(desc, block): Creates a skipped test suite. The block will not be executed.

10. void ttest_beginTest(const char *desc, int failAsPassFlag, int skip): Begins a test case with a description. Allows flags to force pass or skip.

11. void ttest_endTest(): Ends the current test.

12. IT(desc, block): Macro to define a test case.

13. IT_SKIP(desc, block): Defines a test case that will be skipped. The block will not be executed.

14. IT_FAIL(desc, block): Defines a test case where if it fails it will be considered successful and vice versa.

15. int ttest_conclude(): Prints the test results summary. Must be called once at the end. The function also returns an exit code, 1 if any one failed and 0 if all passed.

16. CONCLUDE(): Macro alias for ttest_conclude().

17. int ttest_assert(int expr): Evaluates the expression. If false, the test fails but execution keep continues.

18. ASSERT(expr): Macro alias for ttest_assert(expr).


Example:
----------------------------
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

License:
----------------------------
Tinytest-c is licensed under the MIT License.
