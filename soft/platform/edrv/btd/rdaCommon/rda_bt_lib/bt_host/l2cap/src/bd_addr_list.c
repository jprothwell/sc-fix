
/**************************************************************************
 * MODULE NAME:    bd_addr_list.c
 * PROJECT CODE:
 * DESCRIPTION:    maintains a list of bd addresses.
 * MAINTAINER:     Gary Fleming
 * DATE:           15-06-2000
 *
 * SOURCE CONTROL: 
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 * Description :
 * This module is used by L2CAP layer for the managment of Bd_ADDR lists 
 * used to maintain group members.
 *
 *************************************************************************/

#include "host_config.h"

#include "host_types.h"
#include "papi.h"
#include "l2_utils.h"
#include "bd_addr_list.h"

#if (L2CAP_GROUPS_SUPPORTED==1)

int _Addr_Match(t_bdaddr *bd_1, t_bdaddr *bd_2);

/***************************************************
 *
 * Function :- prh_l2_bda_list_init
 *
 * Description 
 * Initialises the Bd_Addr_List
 *
 ***************************************************/

List* prh_l2_bda_list_init()
{
    List *p_list;
    
    p_list = (List*)pMalloc(sizeof(List));
	if (!p_list)
		return 0;
    p_list->head = 0;
    p_list->tail = 0;
    p_list->length = 0;
    return p_list;
}

/***************************************************
 *
 * Function :- prh_l2_bda_list_insert
 *
 * Description 
 * Inserts a bd address entry into the list.
 *
 ***************************************************/

int  prh_l2_bda_list_insert(List *p_bda_list, t_bdaddr *p_bda)
{
   Node  *p_Node;

   /* If the Device is already contained in the group */
   if(prh_l2_bda_list_contains(p_bda_list,p_bda))
       return 1;

   p_Node = (Node*)pMalloc(sizeof(Node));
   if(!p_Node) return 0;

   p_Node->next = 0;

   if(p_bda_list->length == 0)
   {
       p_bda_list->head = p_bda_list->tail = p_Node;
       p_Node->prev = 0;
   }
   else
   {
      p_bda_list->tail->next = p_Node;
      p_Node->prev = p_bda_list->tail;  
      p_bda_list->tail = p_Node;
   }
   p_bda_list->length++;

   pMemcpy(p_Node->data.bytes, p_bda->bytes,6);

   return 1;
}

/***************************************************
 *
 * Function :- prh_l2_bda_list_remove
 *
 * Description 
 * Removes a bd address entry from the list.
 *
 ***************************************************/
 
int prh_l2_bda_list_remove(List *p_bda_list, t_bdaddr *p_bda)
{
    Node *p_node;

    p_node = prh_l2_bda_list_search(p_bda_list,p_bda);
    if(p_node)
    {
        if (p_node->prev != 0)
            p_node->prev->next = p_node->next;
        if (p_node->next != 0)
            p_node->next->prev = p_node->prev;

        if(p_bda_list->head == p_node) 
            p_bda_list->head = p_node->next;
        if(p_bda_list->tail == p_node) 
            p_bda_list->tail = p_node->prev;
        p_bda_list->length--;
        pFree(p_node);
    }
    return (int)p_node;
}

/***************************************************
 *
 * Function :- prh_l2_bda_list_contains
 *
 * Description 
 * Check if the Queue contains an entry for a given 
 * bd_address.
 *
 ***************************************************/

int prh_l2_bda_list_contains(List *p_bda_list, t_bdaddr *p_bda)
{
    return (int)prh_l2_bda_list_search(p_bda_list,p_bda);
}

/***************************************************
 *
 * Function :- prh_l2_bda_list_search
 *
 * Description 
 * Seaches the Q for a match for a given bd address
 * and retruns the node.
 *
 ***************************************************/

Node* prh_l2_bda_list_search(List *p_bda_list, t_bdaddr *p_bda)
{
    Node *p_node;

    p_node = p_bda_list->head;
    while(p_node != 0)
    {
        if(_Addr_Match(&p_node->data,p_bda))
            return p_node;
        else
            p_node = p_node->next;
    }
    return 0;
}

/***************************************************
 *
 * Function :- prh_l2_bda_list_free
 *
 * Description 
 * Frees the entire Q & alls its entries.
 *
 ***************************************************/

int prh_l2_bda_list_free(List *p_bda_list)
{
    Node *p_node;

    while(p_bda_list->tail !=0)
    {
        p_node = p_bda_list->tail;
        p_bda_list->tail = p_bda_list->tail->prev;
        pFree(p_node);
    }
    pFree(p_bda_list);
    return 1;
}

/***************************************************
 *
 * Function :- _Addr_Match
 *
 * Description 
 * Checks for an address match between two given 
 * device addresses.
 *
 ***************************************************/

int _Addr_Match(t_bdaddr *bd_1, t_bdaddr *bd_2)
{
   return  prh_l2_utils_addr_match(bd_1,bd_2);   
}

#endif /* End of #if (L2CAP_GROUPS_SUPPORTED==1) */
