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

int test_failure(const struct ttest_Test *test) {
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
        IT_FAIL("Failure is expected", &test_failure);
        IT_SKIP("Skipped test", &test_skipped);
    DESCRIBE_END();

    return CONCLUDE();
}
