#include "monopoly.h"

int num_comm_chest_cards = 17;
Card_t community_chest_cards[] = {
    {"Advance to GO (collect $200)",                                               &cc_advance_to_go   },
    {"Bank error in your favor; collect $75.",                                     &bank_error         },
    {"Doctor's fees – Pay $50",                                                    &doctor             },
    {"Get out of jail free – this card may be kept until needed, or sold.",        &cc_get_out_of_jail },
    {"Go directly to jail – Do not pass Go, do not collect $200.",                 &cc_go_to_jail      },
    {"It is your birthday Collect $10 from each player.",                          &birthday           },
    {"Grand Opera Night – collect $50 from every player for opening night seats.", &opera              },
    {"Income Tax refund – collect $20.",                                           &income_tax_refund  },
    {"Life Insurance Matures – collect $100.",                                     &life_insurance     },
    {"Pay Hospital Fees of $100.",                                                 &hospital_fee       },
    {"Pay School Fees of $50.",                                                    &school_fee         },
    {"Receive $25 Consultancy Fee.",                                               &consultancy_fee    },
    {"You are assessed for street repairs – $40 per house, $115 per hotel.",       &street_repairs     },
    {"You have won second prize in a beauty contest – collect $10.",               &beauty_contest     },
    {"You inherit $100.",                                                          &inherit            },
    {"From sale of stock you get $50.",                                            &sale_of_stock      },
    {"Holiday Fund matures - receive $100.",                                       &holiday_fund       }
};

void cc_advance_to_go(Game_state_t * gs_p) {
    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);
    p_p->location_index = GO_L;
    credit_player(p_p, 200);
}

void bank_error(Game_state_t * gs_p) {
    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);
    credit_player(p_p,75);
}

void doctor(Game_state_t * gs_p) {
    int bank = gs_p->num_players;
    indebt_current_player(&(gs_p->turn),bank,50);
}

void cc_get_out_of_jail(Game_state_t * gs_p) {
    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);
    p_p->num_get_out_of_jail_free++;
}

void cc_go_to_jail(Game_state_t * gs_p) {
    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);
    enter_jail(p_p);
}

void birthday(Game_state_t * gs_p) {
}

void opera(Game_state_t * gs_p) {
}

void income_tax_refund(Game_state_t * gs_p) {
}

void life_insurance(Game_state_t * gs_p) {
}

void hospital_fee(Game_state_t * gs_p) {
}

void school_fee(Game_state_t * gs_p) {
}

void consultancy_fee(Game_state_t * gs_p) {
}

void street_repairs(Game_state_t * gs_p) {
}

void beauty_contest(Game_state_t * gs_p) {
}

void inherit(Game_state_t * gs_p) {
}

void sale_of_stock(Game_state_t * gs_p) {
}

void holiday_fund(Game_state_t * gs_p) {
}
