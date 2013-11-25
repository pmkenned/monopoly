#ifndef _COMMUNITY_H_
#define _COMMUNITY_H_

#include "card.h"

extern int num_comm_chest_cards;
extern Card_t community_chest_cards[];

void cc_advance_to_go();
void bank_error();
void doctor();
void cc_get_out_of_jail();
void cc_go_to_jail();
void birthday();
void opera();
void income_tax_refund();
void life_insurance();
void hospital_fee();
void school_fee();
void consultancy_fee();
void street_repairs();
void beauty_contest();
void inherit();
void sale_of_stock();
void holiday_fund();

#endif
