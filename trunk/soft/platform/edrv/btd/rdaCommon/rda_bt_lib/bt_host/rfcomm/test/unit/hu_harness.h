#ifndef HU_HARNESS_H
#define HU_HARNESS_H

#include "host_types.h"

typedef enum {HU_NONE=0, HU_HCI, HU_L2CAP, HU_RFCOMM, HU_SDP, HU_OBEX, HU_TCS} prh_t_hu_group;

typedef enum {HU_PASSED=0, HU_FAILED, HU_UNRESOLVED, HU_UNTESTED} prh_t_hu_outcome;

typedef struct 
{
  const char *name;
  int (*pre_test)(int, void *);
  prh_t_hu_outcome (*execute)(int, void *);
  int (*post_test)(int, void *);
  u_int32 result;
} prh_t_hu_testlist;

typedef struct prh_st_hu_harness
{
  struct prh_st_hu_harness *next;
  prh_t_hu_group group;
  char const *descr;
  u_int32 iterations;
  u_int32 num_tests;
  prh_t_hu_testlist *tests;
  /* total counters per test group */
  u_int32 total_count;
  u_int32 pass_count;
  u_int32 fail_count;
  u_int32 unresolved_count;
  u_int32 untested_count;
} prh_t_hu_harness;

extern prh_t_hu_harness *prh_hu_tests_list_head;

extern u_int32 overall_executed, overall_passed, overall_failed, overall_unresolved, overall_untested;

int prh_hu_add_testgroup(prh_t_hu_harness *new_group);

/* exit codes for tests */
#define PRH_UT_PASSED 0

/* test failed */
#define PRH_UT_FAILED 1

/* something has gone wrong.. not the test itself */
#define PRH_UT_UNRESOLVED 2

/* placeholder for unrunning tests */
#define PRH_UT_UNTESTED 3 

#endif
