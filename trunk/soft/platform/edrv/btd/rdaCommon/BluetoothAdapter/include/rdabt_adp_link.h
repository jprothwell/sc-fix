
#ifndef _RDABT_ADP_LINK_H_

#define _RDABT_ADP_LINK_H_
void rdabt_link_write_scanenable_mode_cnf(ilm_struct *msg, void *mv);
void rdabt_link_discovery_result_ind(ilm_struct *msg, void *mv);
void rdabt_link_inquiry_cancel_cnf(ilm_struct *msg, void *mv);
void rdabt_link_write_localaddr_cnf(ilm_struct *msg, void *mv);
void rdabt_link_write_authentication_cnf(ilm_struct *msg, void *mv);
void rdabt_link_discovery_cnf(ilm_struct *msg, void *mv);
void rdabt_link_pin_code_ind(ilm_struct *msg, void *mv);

void rdabt_link_bonding_result_ind(ilm_struct *msg, void *mv);
void rdabt_link_bonding_cnf(ilm_struct *msg, void *mv);

#endif
