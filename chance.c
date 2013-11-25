#include "chance.h"

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

void c_advance_to_go() {
}

void advance_to_illinois() {
}

void advance_to_utility() {
}

void advance_to_railroad() {
}

void advance_to_stcharles() {
}

void bank_pays_dividend() {
}

void c_get_out_of_jail() {
}

void go_back_three() {
}

void c_go_to_jail() {
}

void general_repairs() {
}

void poor_tax() {
}

void reading_railroad() {
}

void advance_to_boardwalk() {
}

void chairman() {
}

void building_loan() {
}

void crossword() {
}
