
/**************************************************************************
 * MODULE NAME:    bd_addr_list.h
 * PROJECT CODE:
 * DESCRIPTION:    maintains a list of bd addresses.
 * MAINTAINER:     Gary Fleming
 * DATE:           15-06-2000
 *
 * SOURCE CONTROL: 
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 * Description :
 * This module is used by L2CAP layer for the managment of Bd_ADDR lists 
 * used to maintain group members.
 *
 *************************************************************************/

#ifndef BT_ADDR_LIST
#define BT_ADDR_LIST

#include "host_config.h"
#include "host_types.h"
#include "papi.h"

typedef struct list List;
typedef struct node Node;

struct list { 
    int   length;
    Node  *head;
    Node  *tail;
};

struct node {
    t_bdaddr   data;
    Node      *prev;
    Node      *next;
};

Node* prh_l2_bda_list_search(List *bda_list, t_bdaddr *p_bda);
List* prh_l2_bda_list_init(void);
int   prh_l2_bda_list_insert(List *bda_list, t_bdaddr *p_bda);
int   prh_l2_bda_list_remove(List *bda_list, t_bdaddr *p_bda);
int   prh_l2_bda_list_contains(List *bda_list, t_bdaddr *p_bda);
int   prh_l2_bda_list_free(List *p_bda_list);

#endif
