// Defines [Report Hooks](https://criterion.readthedocs.io/en/v2.3.3/hooks.html).
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <criterion/criterion.h>
#include <criterion/hooks.h>


// PRE_ALL: occurs before running the tests.
ReportHook(PRE_ALL)(CR_UNUSED struct criterion_test_set *tests) {
  // Make stdout and stderr unbuffered (taken from the official samples).
  // https://github.com/Snaipe/Criterion/commit/444e39322632a2166eca1cccf1d85e6e97aacb5f
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
}

// PRE_SUITE: occurs before a suite is initialized.
ReportHook(PRE_SUITE)(struct criterion_suite_set *set) {
  printf("\n<DESCRIBE::>%s\n", set->suite.name);
}

// PRE_INIT: occurs before a test is initialized.
ReportHook(PRE_INIT)(CR_UNUSED struct criterion_test *test) {
}

// PRE_TEST: occurs after the test initialization, but before the test is run.
ReportHook(PRE_TEST)(struct criterion_test *test) {
  printf("\n<IT::>%s\n", test->data->description != NULL ? test->data->description : test->name);
}

// ASSERT: occurs when an assertion is hit
ReportHook(ASSERT)(struct criterion_assert_stats *stats) {
  if (stats->passed) {
    puts("\n<PASSED::>Test Passed");
  } else {
    if (stats->message && *stats->message) {
      printf("\n<FAILED::>");
      char *dup = strdup(stats->message);
      char *saveptr = NULL;
      char *line = strtok_r(dup, "\n", &saveptr);
      do {
        printf("%s", line);
        line = strtok_r(NULL, "\n", &saveptr);
        if (line) printf("<:LF:>");
      } while (line);
      free(dup);
      puts("");
    } else {
      puts("\n<FAILED::>Test Failed");
    }
  }
}

// THEORY_FAIL: occurs when a theory iteration fails.
ReportHook(THEORY_FAIL)(CR_UNUSED struct criterion_theory_stats *stats) {
}

// TEST_CRASH: occurs when a test crashes unexpectedly.
ReportHook(TEST_CRASH)(struct criterion_test_stats *stats) {
  printf("\n<ERROR::>Test Crashed<:LF:>Caught unexpected signal: ");
  switch (stats->signal) {
    case SIGILL:
      printf("SIGILL (%d). %s\n", stats->signal, "Invalid instruction.");
      break;

    case SIGFPE:
      printf("SIGFPE (%d). %s\n", stats->signal, "Erroneous arithmetic operation.");
      break;

    case SIGSEGV:
      printf("SIGSEGV (%d). %s\n", stats->signal, "Invalid memory access.");
      break;

    default:
      printf("%d\n", stats->signal);
  }
}

// POST_TEST: occurs after a test ends, but before the test finalization.
ReportHook(POST_TEST)(struct criterion_test_stats *stats) {
  if (stats->timed_out) puts("\n<ERROR::>Test Timed Out");

  printf("\n<COMPLETEDIN::>%.4f\n", stats->elapsed_time*1000);
}

// POST_FINI: occurs after a test finalization.
ReportHook(POST_FINI)(CR_UNUSED struct criterion_test_stats *stats) {
}

// POST_SUITE: occurs before a suite is finalized.
ReportHook(POST_SUITE)(struct criterion_suite_stats *stats) {
  float total_ms = 0;
  for (struct criterion_test_stats *ts = stats->tests; ts; ts = ts->next) {
    total_ms += ts->elapsed_time*1000;
  }
  printf("\n<COMPLETEDIN::>%.4f\n", total_ms);
}

// POST_ALL: occurs after all the tests are done.
ReportHook(POST_ALL)(CR_UNUSED struct criterion_global_stats *stats) {
}
