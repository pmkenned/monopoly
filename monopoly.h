#ifndef _MONOPOLY_H_
#define _MONOPOLY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

#define MAXCHARS 100
#define NUM_LOC 40

enum piece_type_e {SHOE, HORSE, DOG, BATTLE, IRON, SPINNING, TOPHAT, THIMBLE, WHEELBARROW};
typedef enum piece_type_e Piece_type_t;

struct player_s {

    char * name;
    Piece_type_t piece_type;
    int cash;
    int location_index; /* location on board */

    // TODO: mortgaged should be a field?
    struct {
        unsigned short own        : 1;
        unsigned short num_houses : 2;
        unsigned short hotel      : 1;
    } property[NUM_LOC];

    int roll_double_count;
    int in_jail;
    int rolls_in_jail;
    int num_get_out_of_jail_free;
    int rent_left_to_pay;
};

typedef struct player_s Player_t;

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
    int rent[6]; // 0, 1, 2, 3, and 4 houses, and a hotel
    struct {
        unsigned short owned      : 1; /* technically unnecessary since owner could be NULL */
        unsigned short mortgaged  : 1;
        unsigned short num_houses : 2;
        unsigned short hotel      : 1;
    } ownership;
    Player_t * owner; // could be a player index...?
};

typedef struct location_s Location_t;

extern Location_t locations[NUM_LOC];

enum locations_enum {
    GO_L,   MED_L, COM0_L, BAL_L,  INC_L, RRR_L, ORI_L,  CHA0_L, VER_L, CON_L, /* first side */
    JAIL_L, STC_L, ELE_L,  STA_L,  VIR_L, PRR_L, STJ_L,  COM1_L, TEN_L, NYA_L, /* second side */
    PARK_L, KEN_L, CHA1_L, IND_L,  ILL_L, BRR_L, ATL_L,  VEN_L,  H2O_L, MAR_L, /* third side */
    GTJ_L,  PAC_L, NCA_L,  COM2_L, PEN_L, SRR_L, CHA2_L, PAR_L,  LUX_L, BRD_L  /* fourth side */
};

struct game_state_s {
    int num_players;
 
    Player_t * players;
 
    int turn;
    int game_over;
};

typedef struct game_state_s Game_state_t;

struct trade_s {
    Player_t * player_making_offer, * partner;
    int cash_offer;
    int cash_request;
    int properties_offer[NUM_LOC];
    int properties_request[NUM_LOC];
    int get_out_free_card_offer;  // TODO: technically, if a player has two, they can offer two
    int get_out_free_card_request;
};

typedef struct trade_s Trade_t;

void init_game(Game_state_t * gs_p);
void enter_jail(Player_t * p_p);
void do_roll(Game_state_t * gs_p, int * doubles, int * landed_on_unowned);
void offer_trade();
void make_property_trade_list(Player_t * p_p, int * list);
Player_t * monopoly_owner(Location_t * l_p);
void manage_property(Game_state_t * gs_p);
void do_auction(Game_state_t * gs_p, int l_i);
void make_player_owner(Player_t * p_p, int l_i);
void advance_turn(Game_state_t * gs_p, int doubles);
void credit_or_debit_player(Player_t * p_p, int amount);
void advance_token(Player_t * p_p, int die1, int die2);
void roll_dice(int * die1, int * die2);
void land_on_property_action(Game_state_t * gs_p, Player_t * p_p, int l_i);
int calculate_assets(Player_t * p_p); // TODO
int calculate_rent(Game_state_t * gs_p, Location_t * l_p);
void charge_rent(Game_state_t * gs_p, Player_t * p_p, Location_t * l_p);
void do_location_action(Game_state_t * gs_p);
void game_iter(Game_state_t * gs_p);
void game_loop(Game_state_t * gs_p);
void end_game(Game_state_t * gs_p);

#endif
