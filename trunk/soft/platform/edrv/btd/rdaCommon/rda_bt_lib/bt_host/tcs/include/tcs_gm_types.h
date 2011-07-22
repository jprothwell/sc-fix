#ifndef PRH_TCS_GM_TYPES_H
#define PRH_TCS_GM_TYPES_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

struct prh_st_tcs_gm_comp_spec
{
  u_int16 comp_id;
};

#define PRH_TCS_IE_COMP_SPEC_LEN 4

struct prh_st_tcs_gm_config_data
{
  t_bdaddr bd_addr;
  u_int8 internal_num[2];
  u_int8 link_key[16];
};

struct prh_st_tcs_gm_called_party_number
{
  u_int8 type;
  u_int8 plan_id;
  u_int8 num_len;
  u_int8 *num;
};

#define PRH_TCS_IE_CALLED_PN_BASE_LEN 3

struct prh_st_tcs_gm_encode_info
{
  u_int8 *cause;

  u_int16 *clock_offset;
  
  struct prh_st_tcs_gm_called_party_number *calledpn;

  struct prh_st_tcs_gm_config_data *config;
  u_int8 num_conf_ents, current_ent;
  struct prh_st_tcs_gm_comp_spec *compspec;
  /* wug member this is being sent to */
  struct prh_st_tcs_wug_member *wug_mem;

};

#define LOWER_HEADER_LEN 15

int prh_tcs_gm_encode_access_rights_reject(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn);
int prh_tcs_gm_encode_access_rights_accept(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn);
int prh_tcs_gm_encode_access_rights_req(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn);

#endif/* (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/

#endif
