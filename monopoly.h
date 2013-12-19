#ifndef _MONOPOLY_H_
#define _MONOPOLY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

#define MAXCHARS 100
#define NUM_LOC 40
#define MAX_PLAYERS 4
#define MAX_HOUSES 4
#define BAIL_COST 50

extern char buffer[MAXCHARS];

enum locations_enum {
    GO_L,   MED_L, COM0_L, BAL_L,  INC_L, RRR_L, ORI_L,  CHA0_L, VER_L, CON_L, /* first side */
    JAIL_L, STC_L, ELE_L,  STA_L,  VIR_L, PRR_L, STJ_L,  COM1_L, TEN_L, NYA_L, /* second side */
    PARK_L, KEN_L, CHA1_L, IND_L,  ILL_L, BRR_L, ATL_L,  VEN_L,  H2O_L, MAR_L, /* third side */
    GTJ_L,  PAC_L, NCA_L,  COM2_L, PEN_L, SRR_L, CHA2_L, PAR_L,  LUX_L, BRD_L  /* fourth side */
};

enum piece_type_e {SHOE, HORSE, DOG, BATTLE, IRON, SPINNING, TOPHAT, THIMBLE, WHEELBARROW};
typedef enum piece_type_e Piece_type_t;

struct turn_s {
    int player_index;
    int die1, die2;
    int doubles;
    int roll_double_count;
    int has_rolled;
    int on_unowned;
    int in_debt; /* a flag for quick checking */
    int debt_total; /* sum of debts[] */
    int * debts; /* stores debts owed to each of the other players */
};

typedef struct turn_s Turn_t;

struct player_s {

    char * name;
    int index;
    Piece_type_t piece_type;
    int cash;
    int liquid_assets; /* cash + half price of houses/hotels + mortgage value of property */
    int taxable_assets; /* used for income tax */
    int location_index; /* location on board */

    int bankrupt;

    /* NOTE: changed to be only ownership flags for each location */
    int property[NUM_LOC];

    int in_jail;
    int rolls_in_jail;
    int num_get_out_of_jail_free;
};

typedef struct player_s Player_t;

struct game_state_s {
    int num_players;
    int num_active_players; /* excludes bankrupt players */
 
    Player_t * players;
    Player_t * winner;
 
    Turn_t turn;
    int game_over; /* when only one player is not bankrupt */
};

typedef struct game_state_s Game_state_t;

struct card_s {
    char text[200];
    void (*func)(Game_state_t * gs_p);
};

typedef struct card_s Card_t;

extern int num_chance_cards;
extern Card_t chance_cards[];

extern int num_comm_chest_cards;
extern Card_t community_chest_cards[];

enum {TRADE, MANAGE, ROLL, BUY, AUCTION, PAY_DEBT, END, JAIL_ROLL, BAIL, GET_OUT_FREE};

#define NUM_ACTION_STRINGS  10
extern int actions_allowed[NUM_ACTION_STRINGS];
extern char action_strings[][40];

enum location_type_e {
    GO_T,
    PROPERTY_T,
    CHEST_T,
    CHANCE_T,
    INCOME_TAX_T,
    LUXURY_TAX_T,
    JAIL_T,
    GO_TO_JAIL_T,
    PARKING_T};

typedef enum location_type_e Location_type_t;

/* used for indexing rent array */
enum {ZERO_HOUSES=0, ONE_HOUSE, TWO_HOUSES, THREE_HOUSES, FOUR_HOUSES, HOTEL};

/* NOTE: property_groups and the group_names_e enum are corelated.
   if one changes, the other must change accordingly */
#define NUM_PROPERTY_GROUPS 10
extern int property_groups[][5];
enum group_names_e {
    PURPLE,
    GRAY,
    PINK,
    ORANGE,
    RED,
    YELLOW,
    GREEN,
    BLUE,
    RAIL,
    UTILITY,
    NONE
};

typedef enum group_names_e Group_type_t;

struct location_s {
    char name[MAXCHARS];
    Location_type_t location_type;
    Group_type_t group;

    /* rest only for PROPERTY_T */
    int price;
    int house_price;
    int hotel_price;
    int mortgage_value;
    int rent[6]; /* 0, 1, 2, 3, and 4 houses, and a hotel */
    struct {
        unsigned int owned       : 1; /* technically unnecessary since owner could be NULL */
        unsigned int mortgaged  : 1;
        unsigned int num_houses : 3;
        unsigned int hotel      : 1;
    } ownership;
    Player_t * owner; /* could be a player index...? */
};

typedef struct location_s Location_t;

extern Location_t locations[NUM_LOC];

struct trade_s {
    Player_t * player_making_offer, * partner;
    int cash_offer;
    int cash_request;
    int properties_offer[NUM_LOC];
    int properties_request[NUM_LOC];
    int get_out_free_card_offer;  /* TODO: technically, if a player has two, they can offer two */
    int get_out_free_card_request;
};

typedef struct trade_s Trade_t;

void pause();
void init_game(Game_state_t * gs_p);
void print_game_state(Game_state_t * gs_p);
void enter_jail(Player_t * p_p);
void do_roll(Game_state_t * gs_p);
void offer_trade();
void make_property_trade_list(Player_t * p_p, int * list);
Player_t * monopoly_owner(Location_t * l_p);
void manage_property(Game_state_t * gs_p, Player_t * p_p);
void do_auction(Game_state_t * gs_p, int l_i);
void make_player_owner(Player_t * p_p, int l_i);
void advance_turn(Game_state_t * gs_p);

void unmortgage_properties(Player_t * p_p);
void mortgage_properties(Player_t * p_p);
void buy_houses(Player_t * p_p);
void sell_houses(Player_t * p_p);

void declare_bankruptcy(Game_state_t * gs_p);
void pay_off_debts(Game_state_t * gs_p);
void indebt_current_player(Turn_t * t, int creditor, int amount);
void credit_player(Player_t * p_p, int amount);
void debit_player(Player_t * p_p, int amount);

void advance_token(Player_t * p_p, int die1, int die2);
void roll_dice(Game_state_t * gs_p);
void land_on_property_action(Game_state_t * gs_p, Player_t * p_p, int l_i);
int calculate_rent(Game_state_t * gs_p, Location_t * l_p);
void charge_rent(Game_state_t * gs_p, Location_t * l_p);
void do_location_action(Game_state_t * gs_p);
void game_iter(Game_state_t * gs_p);
void game_loop(Game_state_t * gs_p);
void end_game(Game_state_t * gs_p);

void c_advance_to_go(Game_state_t * gs_p);
void advance_to_illinois(Game_state_t * gs_p);
void advance_to_utility(Game_state_t * gs_p);
void advance_to_railroad(Game_state_t * gs_p);
void advance_to_stcharles(Game_state_t * gs_p);
void bank_pays_dividend(Game_state_t * gs_p);
void c_get_out_of_jail(Game_state_t * gs_p);
void go_back_three(Game_state_t * gs_p);
void c_go_to_jail(Game_state_t * gs_p);
void general_repairs(Game_state_t * gs_p);
void poor_tax(Game_state_t * gs_p);
void reading_railroad(Game_state_t * gs_p);
void advance_to_boardwalk(Game_state_t * gs_p);
void chairman(Game_state_t * gs_p);
void building_loan(Game_state_t * gs_p);
void crossword(Game_state_t * gs_p);

void cc_advance_to_go(Game_state_t * gs_p);
void bank_error(Game_state_t * gs_p);
void doctor(Game_state_t * gs_p);
void cc_get_out_of_jail(Game_state_t * gs_p);
void cc_go_to_jail(Game_state_t * gs_p);
void birthday(Game_state_t * gs_p);
void opera(Game_state_t * gs_p);
void income_tax_refund(Game_state_t * gs_p);
void life_insurance(Game_state_t * gs_p);
void hospital_fee(Game_state_t * gs_p);
void school_fee(Game_state_t * gs_p);
void consultancy_fee(Game_state_t * gs_p);
void street_repairs(Game_state_t * gs_p);
void beauty_contest(Game_state_t * gs_p);
void inherit(Game_state_t * gs_p);
void sale_of_stock(Game_state_t * gs_p);
void holiday_fund(Game_state_t * gs_p);

#endif
