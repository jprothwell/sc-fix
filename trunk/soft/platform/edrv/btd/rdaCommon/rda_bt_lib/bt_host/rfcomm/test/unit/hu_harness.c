#include <stdio.h>

#include "hu_harness.h"

u_int32 overall_executed, overall_passed, overall_failed, overall_unresolved, overall_untested;
prh_t_hu_harness *prh_hu_tests_list_head;
int prh_hu_tests_count;

int main(void)
{
  int i;
  int num_harness;
  prh_t_hu_harness *current;
  prh_t_hu_outcome outcome;
  
  pInit();
  prh_rfcomm_layer_ctrl(PRH_HOST_BOOT);
  rftest_init_events();

  register_rfcomm_tests();
  current=prh_hu_tests_list_head;
  
  for ( ; current!=NULL; current=current->next)
    {
      printf("%s - number to execute %d\n", current->descr, current->num_tests);    
      for (i=0; i<current->num_tests; i++)
	{
	  if (current->tests[i].pre_test)
	    current->tests[i].pre_test(0, NULL);

	  if (current->tests[i].execute)
	    outcome=current->tests[i].execute(0, NULL);

	  if (current->tests[i].post_test)
	    current->tests[i].post_test(0, NULL);
	  
	  switch(outcome)
	    {
	    case HU_PASSED:
	      current->pass_count++;
	      overall_passed++;
	      break;
	    case HU_FAILED:
	      printf("\n%s failed\n\n", current->tests[i].name);
	      current->fail_count++;
	      overall_failed++;
	      break;
	    case HU_UNRESOLVED:
	      current->unresolved_count++;
	      overall_unresolved++;
	      break;
	    case HU_UNTESTED:
	      current->untested_count++;
	      overall_untested++;
	      break;
	    }
	  current->total_count++;
	  overall_executed++;
	}
      printf("%s - executed %d:  %d passed %d failed\n", current->descr, current->total_count, current->pass_count, current->fail_count);
    }

  printf("Overall - executed %d: %d passed %d failed\n", overall_executed, overall_passed, overall_failed);
  return 0;
}
  
int prh_hu_add_testgroup(prh_t_hu_harness *new_group)
{
  prh_t_hu_harness *trav;
  
  trav=prh_hu_tests_list_head;

  new_group->next=NULL;
  
  if (trav==NULL)
    prh_hu_tests_list_head=new_group;
  else
    {
      while(trav->next!=NULL)
	trav=trav->next;
      
      trav->next=new_group;
    }
  prh_hu_tests_count++;
  return 0;
}
