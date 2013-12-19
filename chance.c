#include "monopoly.h"

int num_chance_cards = 17;
Card_t chance_cards[] = {
    {"Advance to Go (Collect $200).",                                                                                                                                      &c_advance_to_go      },
    {"Advance to Illinois Ave.",                                                                                                                                           &advance_to_illinois  },
    {"Advance token to nearest Utility. If unowned, you may buy it from the Bank. If owned, throw dice and pay owner a total ten times the amount thrown.",                &advance_to_utility   },
    {"Advance token to the nearest Railroad and pay owner twice the rental to which he/she is otherwise entitled. If Railroad is unowned, you may buy it from the Bank.",  &advance_to_railroad  },
    {"Advance token to the nearest Railroad and pay owner twice the rental to which he/she is otherwise entitled. If Railroad is unowned, you may buy it from the Bank.",  &advance_to_railroad  },
    {"Advance to St. Charles Place – if you pass Go, collect $200.",                                                                                                       &advance_to_stcharles },
    {"Bank pays you dividend of $50.",                                                                                                                                     &bank_pays_dividend   },
    {"Get out of Jail free – this card may be kept until needed, or traded/sold.",                                                                                         &c_get_out_of_jail    },
    {"Go back 3 spaces.",                                                                                                                                                  &go_back_three        },
    {"Go directly to Jail – do not pass Go, do not collect $200.",                                                                                                         &c_go_to_jail         },
    {"Make general repairs on all your property – for each house pay $25 – for each hotel $100.",                                                                          &general_repairs      },
    {"Pay poor tax of $15.",                                                                                                                                               &poor_tax             },
    {"Take a trip to Reading Railroad – if you pass Go collect $200.",                                                                                                     &reading_railroad     },
    {"Take a walk on the Boardwalk – advance token to Boardwalk.",                                                                                                         &advance_to_boardwalk },
    {"You have been elected chairman of the board – pay each player $50.",                                                                                                 &chairman             },
    {"Your building loan matures – collect $150.",                                                                                                                         &building_loan        },
    {"You have won a crossword competition - collect $100.",                                                                                                                &crossword            }
};

void c_advance_to_go(Game_state_t * gs_p) {
    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);
    p_p->location_index = GO_L;
    credit_player(p_p, 200);
}

void advance_to_illinois(Game_state_t * gs_p) {
    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);
    p_p->location_index = ILL_L;
}

void advance_to_utility(Game_state_t * gs_p) {
    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);
    int l_i;
    int owned;
    int dice_sum;
    if(p_p->location_index < ELE_L) {
        printf("Advancing to the Electric Company\n");
        l_i = p_p->location_index = ELE_L;
    }
    else {
        printf("Advancing to Water Works\n");
        l_i = p_p->location_index = H2O_L;
    }
    owned = locations[l_i].ownership.owned;
    if(owned) {
        roll_dice(gs_p);
        dice_sum = gs_p->turn.die1 + gs_p->turn.die2;
        debit_player(p_p, 10*dice_sum);
    }
    else {
        printf("Would you like to buy the utility? ");
        fgets(buffer,MAXCHARS, stdin);
        if(strncmp(buffer,"y",1) == 0) {
            printf("TODO\n");
        }
    }
}

void advance_to_railroad(Game_state_t * gs_p) {
}

void advance_to_stcharles(Game_state_t * gs_p) {
    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);
    p_p->location_index = STC_L;
    /* TODO: do_location_action? */
}

void bank_pays_dividend(Game_state_t * gs_p) {
    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);
    credit_player(p_p,50);
}

void c_get_out_of_jail(Game_state_t * gs_p) {
}

void go_back_three(Game_state_t * gs_p) {
}

void c_go_to_jail(Game_state_t * gs_p) {
}

void general_repairs(Game_state_t * gs_p) {
}

void poor_tax(Game_state_t * gs_p) {
}

void reading_railroad(Game_state_t * gs_p) {
}

void advance_to_boardwalk(Game_state_t * gs_p) {
}

void chairman(Game_state_t * gs_p) {
}

void building_loan(Game_state_t * gs_p) {
}

void crossword(Game_state_t * gs_p) {
}
