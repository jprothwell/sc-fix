/***********************************************************************
 *
 * MODULE NAME:    host_ll.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Simple Host Generic Linked List (used by RFCOMM/TCS/TCI)
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  21 Aug 2000
 *
 * SOURCE CONTROL: $Id: host_ll.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
#include "host_config.h"

#include "host_types.h"
#include "host_error.h"

#if (PRH_HOST_UNIT_TEST==1)
#include "stubs/unit_stub_papi.h"
#else
#include "papi.h"
#endif

#include "host_ll.h"

//#define COMBINED_HOST 1

#if COMBINED_HOST==1
#define MAX_HOST_LINK_COUNT		60
struct prh_host_gen_ll host_ll_links[MAX_HOST_LINK_COUNT];
#endif
/*
 * prh_host_ll_add
 * -- Generic Linked List add function.
 *
 * Adds a member to a generic linked list
 */
int prh_host_ll_add(struct prh_host_gen_ll **list, void *data)
{
	struct prh_host_gen_ll *trav, *newent=NULL;
#if COMBINED_HOST==1
	int i;

	for(i=0; i<MAX_HOST_LINK_COUNT; i++)
	{
		if(host_ll_links[i].flag == 0)
		{
			host_ll_links[i].flag = 1;
			newent = &(host_ll_links[i]);
			break;
		}
	}
#else  
  newent=(struct prh_host_gen_ll *)pMalloc(sizeof(struct prh_host_gen_ll));
#endif
  if (newent==pNULL)
    return BT_NORESOURCES;
  newent->next=NULL;
  newent->data=data;

  trav=*list;
  
  if (trav==NULL)
    {
      *list=newent;
    }
  else
    {
      while(trav->next!=NULL)
    trav=trav->next;

      trav->next=newent;
      
    }
  return BT_NOERROR;
}

/*
 * prh_remove_ll_remove
 * -- Generic Linked List remove function
 *
 * remove a member from a linked list by using the helper compare function
 */
int prh_host_ll_remove(struct prh_host_gen_ll **list, void **data, void *cmp_input, prh_host_t_cmp_funcptr cmp_funcptr)
{
  struct prh_host_gen_ll *trav, *prev;

  if (*list==pNULL)
    return BT_INVALIDPARAM;
  trav=prev=*list;
  
  while (trav->next!=pNULL && !((*cmp_funcptr)(cmp_input, trav->data)))
    {
      prev=trav;
      trav=trav->next;
    }
  
  if ((*cmp_funcptr)(cmp_input, trav->data))
    {
      if (trav==*list)
    {
      *list=trav->next;
    }
      else
    {
      prev->next=trav->next;
    }

      *data=trav->data;
#if COMBINED_HOST==1
      trav->flag = 0;
#else  
      pFree(trav);
#endif
      return BT_NOERROR;
    }
  else
    return BT_INVALIDPARAM;
}

/*
 * prh_host_ll_search
 * -- Generic Linked List Search for a member
 *
 * Search a linked list for a member using the helper function
 */
int prh_host_ll_search(struct prh_host_gen_ll **list, void **data,  void *cmp_input, prh_host_t_cmp_funcptr cmp_funcptr)
{
   struct prh_host_gen_ll *trav;
  
   trav=*list;

   if (trav==pNULL)
     return BT_INVALIDPARAM;
   
   while ((trav->next != pNULL) && !((*cmp_funcptr)(cmp_input, trav->data)))
     trav=trav->next;
   
   if ((*cmp_funcptr)(cmp_input, trav->data))
     {
       *data=trav->data;
       return BT_NOERROR;
     }
   else
     return BT_INVALIDPARAM;
   
}

/*
 * prh_host_ll_traverse
 * -- Generic linked list traversal
 *
 * Execute the execute helper function on all members of the linked list
 */
int prh_host_ll_traverse(struct prh_host_gen_ll *list, prh_host_t_exe_funcptr execute_funcptr, void *func_data)
{
  struct prh_host_gen_ll *trav;

  trav=list;

  if (trav==pNULL)
    return BT_NOERROR;

  do
    {
      (*execute_funcptr)(trav->data, func_data);
      trav=trav->next;
    } while(trav!=pNULL);
  
  return 0;
}

/*
 * prh_host_ll_remove_all
 * -- Generic Linked List member removal function
 *
 * Remove all members from the linked list caalling execute funcptr on them
 */
int prh_host_ll_remove_all(struct prh_host_gen_ll **list, prh_host_t_exe_funcptr execute_funcptr, void *func_data)
{
  struct prh_host_gen_ll *trav, *prev;

  trav=prev=*list;

  if (trav==pNULL)
    return BT_NOERROR;

  do
    {
      prev=trav;
      trav=trav->next;
      if (execute_funcptr)
    (*execute_funcptr)(prev->data, func_data);
#if COMBINED_HOST==1
      prev->flag = 0;
#else  
      pFree(prev);
#endif
    } while(trav!=pNULL);
  
  *list=pNULL;
  return 0;
}
