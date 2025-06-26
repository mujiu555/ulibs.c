
#include "ima.h"
#include "ept.h"
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <string.h>

void *_int_index(const void *obj, size_t idx, void *_) {
  _ept__unused(_);
  return (int *)obj + idx;
}
int _int_cmp(const void *a, const void *b, void *_) {
  _ept__unused(_);
  return *(int *)a - *(int *)b;
}

void dima(ima obj) {
  FOREACH(int, i, obj->data, obj->head.len) { printf("%d ", *i); }
  puts("");
}

void test_init_int(void) {
  int a[] = {21, 32, 13, 7, 1, 21, 3, 2, 6, 9, 5, 26, 13};
  ima i   = ima_init(ARRLEN(a), sizeof(int), a);

  CU_ASSERT_EQUAL(i->head.len, ARRLEN(a));
  CU_ASSERT_EQUAL(i->head.cap, ARRLEN(a) * sizeof(int));
  CU_ASSERT_EQUAL(i->head.size, sizeof(int));
  CU_ASSERT_EQUAL(i->head.flag, 0);
  CU_ASSERT(!memcmp(a, i->data, i->head.cap));
}

static CU_TestInfo testcases[] = {
  {"test_ima_init(int)", test_init_int},
  CU_TEST_INFO_NULL,
};

int          suite_success_init() { return 0; }
int          suite_success_clean() { return 0; }
CU_SuiteInfo suites[] = {
  {"initial", suite_success_init, suite_success_clean, NULL, NULL, testcases},
  CU_SUITE_INFO_NULL,
};

void AddTests() {
  assert(NULL != CU_get_registry());
  assert(!CU_is_test_running());

  if (CUE_SUCCESS != CU_register_suites(suites)) {
    exit(EXIT_FAILURE);
  }
}

int main(int _1, char *_2[], char *_3[]) {
  _ept__unused(_1);
  _ept__unused(_2);
  _ept__unused(_3);

  if (CU_initialize_registry()) {
    exit(EXIT_FAILURE);
  } else {
    AddTests();
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    //CU_console_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
  }

  return EXIT_SUCCESS;
}
