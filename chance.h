#ifndef _CHANCE_H_
#define _CHANCE_H_

#include "card.h"

extern int num_chance_cards;
extern Card_t chance_cards[];

void c_advance_to_go();
void advance_to_illinois();
void advance_to_utility();
void advance_to_railroad();
void advance_to_stcharles();
void bank_pays_dividend();
void c_get_out_of_jail();
void go_back_three();
void c_go_to_jail();
void general_repairs();
void poor_tax();
void reading_railroad();
void advance_to_boardwalk();
void chairman();
void building_loan();
void crossword();

#endif
