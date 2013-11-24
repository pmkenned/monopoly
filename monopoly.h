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
    int has_get_out_of_jail_free;
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

struct location_s {
    char name[MAXCHARS];
    Location_type_t location_type;

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

enum locations_enum {GO_L =0, MED_L=1};

struct game_state_s {
    int num_players;
 
    Player_t * players;
 
    int turn;
    int game_over;
};

typedef struct game_state_s Game_state_t;

void init_game(Game_state_t * gs_p);
void enter_jail(Player_t * p_p);
void do_roll(Game_state_t * gs_p, int * doubles, int * landed_on_unowned);
void offer_trade();
void manage_property(Game_state_t * gs_p);
void do_auction(Game_state_t * gs_p);
void advance_turn(Game_state_t * gs_p, int doubles);
void credit_or_debit_player(Game_state_t * gs_p, Player_t * p_p, int amount);
void advance_token(Game_state_t * gs_p, int die1, int die2);
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
