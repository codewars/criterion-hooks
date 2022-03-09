#include <criterion/criterion.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int echo(int);

Test(Basic, passing_test) {
  cr_assert_eq(echo(42), 42);
}

Test(Basic, failing_test) {
  cr_assert_eq(echo(42), 13);
}

Test(Large, random_tests) {
  srand(time(NULL));
  static int inputs[] = { 13, 42 };
  for (int i = 0; i < 20; ++i) {
    int input = inputs[rand() & 1];
    cr_assert_eq(echo(input), 42, "Incorrect answer for n=%d", input);
  }
}

Test(Large, passing_tests) {
  for (int i = 0; i < 100; ++i) {
    cr_assert_eq(echo(i), i);
  }
}

Test(Mixed, tests_with_mixed_expect_and_assert) {
  cr_expect_eq(echo(13), 7);
  cr_expect_eq(echo(42), 7, "Incorrect answer for n=42");
  cr_assert_eq(echo(7), 0);
}

Test(Noisy, output_sync) {
  cr_expect(0, "Failed expect begin");
  printf("%i\n", 0);
  printf("%i\n", 1);
  printf("%i\n", 2);
  printf("%i\n", 3);
  printf("%i\n", 4);
  cr_expect(0, "Failed expect middle");
  printf("%i\n", 5);
  printf("%i\n", 6);
  printf("%i\n", 7);
  printf("%i\n", 8);
  printf("%i\n", 9);
  cr_expect(0, "Failed expect end");
}
