
/* 
    tests should only include this header and any internal test/test group headers 
    - no other stack headers should be included
*/

#include "bt.h"
#include "papi.h"


int Test_Print(char *formatstring, ...);


#define TEST_PASS		0x01
#define TEST_FAIL		0x02
#define TEST_UNKNOWN	0x03 /* (i.e. the remote device will decide pass/fail) */
#define TEST_ERROR		0x04


#define PARAM_UNUSED 0x00

struct testEntry {
    char *testName;
    u_int32(*testFunc)(t_bdaddr remoteAddress, u_int32 groupParm, u_int32 testParam);
    u_int32 testParam; /* allows a function to implement more than one test */
    u_int32 flags; /* see TEST_xx below */
    char *comment; /* if present, is displayed in the dialog box when the test is selected */
};

#define TEST_NOFLAGS            0x00
#define TEST_UPPER_INITIATED	0x01 /* test must be running on lower tester first */
#define TEST_LOWER_INITIATED	0x02 /* test must be running on upper tester first */
#define TEST_LOCAL_ONLY			0x03 /* uses the local device only - does not use remoteAddress */
#define TEST_NOT_BATCHED		0x04 /* test only run via manual selection, not batch run */
#define TEST_RESET_SYSTEM			0x08 /* reset the host after the test returns */
    

	
struct groupEntry {
    char *groupName;
    struct testEntry *testList;
    u_int32 groupParam; /* this param is passed into the test when run (mainly for SDP test 'modes') */
    u_int32 flags; /* see GROUP_xx below */
    char *comment; /* if present, is displayed in the dialog box when the group is selected */
};

#define GROUP_NOFLAGS       0x00
#define GROUP_INTERNAL		0x01 /* internal parthus tests - may need manual intervention/special stack/etc. */
#define GROUP_NOT_BATCHED	0x02 /* tests only run via manual selection, not batch run */
#define GROUP_RESET_HOST	0x04 /* reset the host after the group completes */
#define GROUP_RESET_HC		0x08 /* reset the HC after the group completes */




	







