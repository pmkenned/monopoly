#include "community.h"

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

void cc_advance_to_go() {
}

void bank_error() {
}

void doctor() {
}

void cc_get_out_of_jail() {
}

void cc_go_to_jail() {
}

void birthday() {
}

void opera() {
}

void income_tax_refund() {
}

void life_insurance() {
}

void hospital_fee() {
}

void school_fee() {
}

void consultancy_fee() {
}

void street_repairs() {
}

void beauty_contest() {
}

void inherit() {
}

void sale_of_stock() {
}

void holiday_fund() {
}
