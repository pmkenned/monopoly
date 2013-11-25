#include "monopoly.h"

/* major TODO items:
 * chance and community chest cards
 * finish trade functionality
 * allow players to manage property
 */

/*#define DEBUG*/

#define min(x,y) (x) < (y) ? (x) : (y)

const char equals[] = "================================================";

/* first number is the number of properties in the group */
#define NUM_PROPERTY_GROUPS 10
int property_groups[][5] = {
    {2, MED_L, BAL_L, 0,     0},
    {3, ORI_L, VER_L, CON_L, 0},
    {3, STC_L, STA_L, VIR_L, 0},
    {3, STJ_L, TEN_L, NYA_L, 0},
    {3, KEN_L, IND_L, ILL_L, 0},
    {3, ATL_L, VEN_L, MAR_L, 0},
    {3, PAC_L, NCA_L, PEN_L, 0},
    {2, PAR_L, BRD_L, 0,     0},
    {4, RRR_L, PRR_L, BRR_L, SRR_L},
    {2, ELE_L, H2O_L, 0,     0}
};

Location_t locations[NUM_LOC] =
{
  /* abbreviation key
prices:
   h = house cost
   H = hotel cost
   m = mortgage value
rent:
   [0-4] = rent with [0-4] houses
   H = rent with hotel
ownership:
   o = own
   m = mortgaged
   h = num houses
   H = hotel
  */
  /*                                                      |--prices--|  |-----------rent---------|  |-ownership-|       */
  /* name                    type          group      p    h    H   m    0   1   2    3    4    H    o  m  h  H   owner */
    {"GO",                   GO_T,         NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Mediterranean Avenue", PROPERTY_T,   PURPLE,   60,  50,  50, 30, { 2, 10, 30,  90, 160, 250}, {0, 0, 0, 0}, NULL},
    {"Community Chest",      CHEST_T,      NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Baltic Avenue",        PROPERTY_T,   PURPLE,   60,  50,  50, 30, { 4, 20, 60, 180, 320, 450}, {0, 0, 0, 0}, NULL},
    {"Income Tax",           INCOME_TAX_T, NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Reading Railroad",     PROPERTY_T,   RAIL,    200,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Oriental Avenue",      PROPERTY_T,   GRAY,    100,  50,  50, 50, { 6, 30, 90, 270, 400, 550}, {0, 0, 0, 0}, NULL},
    {"Chance",               CHANCE_T,     NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Vermont Avenue",       PROPERTY_T,   GRAY,    100,  50,  50, 50, { 6, 30, 90, 270, 400, 550}, {0, 0, 0, 0}, NULL},
    {"Connecticut Avenue",   PROPERTY_T,   GRAY,    120,  50,  50, 60, { 8, 40,100, 300, 450, 600}, {0, 0, 0, 0}, NULL},
    {"Jail",                 JAIL_T,       NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"St. Charles Place",    PROPERTY_T,   PINK,    140, 100, 100, 70, {10, 50,150, 450, 625, 750}, {0, 0, 0, 0}, NULL},
    {"Electric Company",     PROPERTY_T,   UTILITY, 150,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"States Avenue",        PROPERTY_T,   PINK,    140, 100, 100, 70, {10, 50,150, 450, 625, 750}, {0, 0, 0, 0}, NULL},
    {"Virginia Avenue",      PROPERTY_T,   PINK,    160, 100, 100, 80, {12, 60,180, 500, 700, 900}, {0, 0, 0, 0}, NULL},
    {"Pennsylvania Railroad",PROPERTY_T,   RAIL,    200,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"St. James Place",      PROPERTY_T,   ORANGE,  180, 100, 100, 90, {14, 70,200, 550, 750, 950}, {0, 0, 0, 0}, NULL},
    {"Community Chest",      CHEST_T,      NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Tennessee Avenue",     PROPERTY_T,   ORANGE,  180, 100, 100, 90, {14, 70,200, 550, 750, 950}, {0, 0, 0, 0}, NULL},
    {"New York Avenue",      PROPERTY_T,   ORANGE,  200, 100, 100,100, {16, 80,220, 600, 800,1000}, {0, 0, 0, 0}, NULL},
    {"Free Parking",         PARKING_T,    NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Kentucky Avenue",      PROPERTY_T,   RED,     220, 150, 150,110, {18, 90,250, 700, 875,1050}, {0, 0, 0, 0}, NULL},
    {"Chance",               CHANCE_T,     NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Indiana Avenue",       PROPERTY_T,   RED,     220, 150, 150,110, {18, 90,250, 700, 875,1050}, {0, 0, 0, 0}, NULL},
    {"Illinois Avenue",      PROPERTY_T,   RED,     240, 150, 150,120, {20,100,400, 750, 925,1100}, {0, 0, 0, 0}, NULL},
    {"B.&O. Railroad",       PROPERTY_T,   RAIL,    200,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Atlantic Avenue",      PROPERTY_T,   YELLOW,  260, 150, 150,130, {22,110,330, 800, 975,1150}, {0, 0, 0, 0}, NULL},
    {"Ventnor Avenue",       PROPERTY_T,   YELLOW,  260, 150, 150,130, {22,110,330, 800, 975,1150}, {0, 0, 0, 0}, NULL},
    {"Water Works",          PROPERTY_T,   UTILITY, 150,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Marvin Gardens",       PROPERTY_T,   YELLOW,  280, 150, 150,140, {24,120,360, 850,1025,1150}, {0, 0, 0, 0}, NULL},
    {"GO TO JAIL",           GO_TO_JAIL_T, NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Pacific Avenue",       PROPERTY_T,   GREEN,   300, 200, 200,150, {26,130,390, 900,1100,1275}, {0, 0, 0, 0}, NULL},
    {"North Carolina Avenue",PROPERTY_T,   GREEN,   300, 200, 200,150, {26,130,390, 900,1100,1275}, {0, 0, 0, 0}, NULL},
    {"Community Chest",      CHEST_T,      NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Pennsylvania Avenue",  PROPERTY_T,   GREEN,   320, 200, 200,160, {28,150,450,1000,1200,1400}, {0, 0, 0, 0}, NULL},
    {"Short Line",           PROPERTY_T,   RAIL,    200,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Chance",               CHANCE_T,     NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Park Place",           PROPERTY_T,   BLUE,    350, 200, 200,175, {35,175,500,1100,1300,1500}, {0, 0, 0, 0}, NULL},
    {"Luxury Tax",           LUXURY_TAX_T, NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Boardwalk",            PROPERTY_T,   BLUE,    400, 200, 200,200, {50,200,600,1400,1700,2000}, {0, 0, 0, 0}, NULL},
};

void chomp(char * s) {
    int len = strlen(s)-1;

    while(s[len] == '\n' && len > 0) {
        len--;
    }
    s[len+1] = '\0';
}

void init_game(Game_state_t * gs_p) {

    int i;
    int turn;
    int num_players;
    char buffer[MAXCHARS];

    srand(time(NULL)); /* initialize random number generator */

    gs_p->game_over = 0;

    printf("%s\n",equals);
    printf("GAME SETUP\n");
    printf("%s\n",equals);

    /* prompt for number of players */
    do {
        printf("Enter number of players: ");
        fgets(buffer,MAXCHARS,stdin);
        num_players = atoi(buffer);
    } while(num_players < 2 || num_players > 4);

    gs_p->num_players = num_players;
    gs_p->players = malloc(sizeof(Player_t)*num_players); /* create players */

    /* init each player */
    for(i=0; i < num_players; i++) {
        Player_t * p_p = &(gs_p->players[i]);
        char ** name = &(p_p->name);
        int name_len;
        int j;

        /* allow each player to pick a name and piece type */
        printf("Enter name for player %d: ", i);
        fgets(buffer,MAXCHARS,stdin);
        chomp(buffer);
        name_len = strlen(buffer);
        *name = malloc(sizeof(char)*(name_len+1));
        strncpy(*name,buffer,name_len);

/*        printf("piece type: ");
        fgets(buffer,MAXCHARS,stdin);
        chomp(buffer); */
        p_p->piece_type = 0; /* TODO */

        p_p->cash = 1500;
        p_p->location_index = GO_L;
        p_p->in_jail = 0;
        p_p->roll_double_count = 0;
        p_p->rent_left_to_pay = 0;
        for(j=0; j<NUM_LOC; j++) {
            p_p->property[j].own = 0;
            p_p->property[j].num_houses = 0;
            p_p->property[j].hotel = 0;
        }
        p_p->num_get_out_of_jail_free = 0;
    }

    /* FOR TESTING: initially give all property to player 0 */
#ifdef DEBUG
    Player_t * master = &(gs_p->players[0]);
    for(i=0; i < NUM_LOC; i++) {
        if(locations[i].location_type == PROPERTY_T)
            make_player_owner(master, i);
    }
#endif

    /* choose random starting player */
    turn = rand() % num_players;
    printf("\nRandom starting player is %s\n",gs_p->players[turn].name);
    gs_p->turn = turn;
}

void advance_turn(Game_state_t * gs_p, int doubles) {
    int * turn = &(gs_p->turn);
    int num_players = gs_p->num_players;
    int i;
    Player_t * current_player_p, * p_p;
    char * p_name;
    int name_len;
    char last_char;

    current_player_p = &(gs_p->players[*turn]);

   /* NOTE: by this point, turn has already ended.
      Thus, if player is in jail, the opportunity for doubles
      to get them out of jail has already passed */
    if(!doubles || current_player_p->in_jail)
        *turn = (*turn + 1) % num_players;

    p_p = &(gs_p->players[*turn]);

    p_name = p_p->name;
    printf("%s\n",equals);
    name_len = strlen(p_name);
    last_char = p_name[name_len-1];
    if(last_char == 's')
        printf("it is now %s' turn\n",p_name);
    else
        printf("it is now %s's turn\n",p_name);
    printf("%s\n",equals);
    printf("Assets:\n");
    printf("$%d cash on hand\n",p_p->cash);
    printf("Properties: \n");
    /* TODO: print houses, hotels, mortgaged */
    for(i=0; i<NUM_LOC; i++) {
        if(p_p->property[i].own)
            printf("%s\n",locations[i].name);
    }
}

/* TODO: disallow negative balances */
void credit_or_debit_player(Player_t * p_p, int amount) {
    int coh;
    char * p_name = p_p->name;
    p_p->cash += amount;
    coh = p_p->cash;
    if(amount > 0)
        printf("%s, your account has been credited $%d. ", p_name, amount);
    else
        printf("%s, your account has been debited $%d. ", p_name, -1*amount);
    printf("You now have $%d.\n",coh);
}

void advance_token(Player_t * p_p, int die1, int die2) {

    int * l_i = &(p_p->location_index);
    int sum = die1 + die2;
    int new_l_i = (*l_i+sum) % NUM_LOC;
    int i;

    for(i=1; i<sum+1; i++) {
        int loc = (*l_i+i) % NUM_LOC;
        if(i==sum)
            printf("YOU LANDED ON: --->%s<---\n",locations[loc].name);
        else
            printf("                   %s\n",locations[loc].name);
        if(loc == GO_L) {
            printf("You passed GO! Collect $200!\n");
            credit_or_debit_player(p_p,200);
        }
    }
    printf("\n");

    (*l_i) = new_l_i;
}

void roll_dice(int * die1, int * die2) {
    *die1 = (rand() % 6)+1;
    *die2 = (rand() % 6)+1;
}

int calculate_assets(Player_t * p_p) {
    return p_p->cash; /* TODO: add in mortgage value, houses, hotels, ...*/
}

int calculate_rent(Game_state_t * gs_p, Location_t * l_p) {
/*    int type       = l_p->location_type; */
    int owned      = l_p->ownership.owned;
    int mortgaged  = l_p->ownership.mortgaged;
    int num_houses = l_p->ownership.num_houses;
    int hotel      = l_p->ownership.hotel;

    int rent, monopoly;
    int rent_index;

    /* TODO: create assertion that the type is PROPERTY_T */

    if(!owned || mortgaged)
        return 0;

    if(!hotel)
        rent_index = num_houses;
    else
        rent_index = HOTEL;

    rent = l_p->rent[rent_index];

    monopoly = (monopoly_owner(l_p) != NULL);

    if(l_p->group == RAIL) {
        /* TODO: rent depends on how many are owned*/
    }
    else if(l_p->group == UTILITY) {
        /* TODO: rent depends on how many are owned*/
    }
    else {
        if(monopoly && num_houses == 0) {
            printf("this property is part of a monopoly; rent is doubled\n");
            rent *= 2;
        }
    }

    return rent;
}

Player_t * monopoly_owner(Location_t * l_p) {

    Player_t * owner = l_p->owner;
    int is_monopoly = 1;

    int group = l_p->group;
    int num_properties_in_group = property_groups[group][0];
    int i;

    if(!l_p->ownership.owned)
        return NULL;

    for(i = 1; i < num_properties_in_group+1; i++) {
        int prop_i = property_groups[group][i];
        Location_t * prop_p = &(locations[prop_i]);
        if(prop_p->owner != owner) {
            is_monopoly = 0;
            break;
        }
    }

    if(is_monopoly)
        return owner;
    else
        return NULL;
}

void charge_rent(Game_state_t * gs_p, Player_t * p_p, Location_t * l_p) {

    Player_t * owner;
    int sufficient_funds;

    int rent = calculate_rent(gs_p, l_p);
    printf("rent is: $%d\n",rent);

    owner = l_p->owner;

    sufficient_funds = (p_p->cash >= rent);
    if(sufficient_funds) {
        credit_or_debit_player(p_p, -1*rent);
        credit_or_debit_player(owner, rent);
        p_p->rent_left_to_pay = 0;
    }
    else {
        printf("YOU HAVE INSUFFICIENT FUNDS; you may raise money by offering a trade or by selling or mortgaging property\n");
        p_p->rent_left_to_pay = rent;
    }

}

void land_on_property_action(Game_state_t * gs_p, Player_t * p_p, int l_i) {
    Location_t * l_p = &(locations[l_i]);

    int owned = l_p->ownership.owned;
    if(owned) {
        Player_t * owner = l_p->owner;
        int owned_by_another = (owner == p_p) ? 0 : 1;
        if(owned_by_another) {
            printf("Property is owned by another player.\n");
            charge_rent(gs_p, p_p, l_p);
        }
        else
            printf("(You own this property)\n");
    }
    else {
        printf("\nYou have landed on an unowned property.\nYou must buy it or it will be auctioned.\n");
    }

}

void do_location_action(Game_state_t * gs_p) {

    int turn = gs_p->turn;
    Player_t * p_p = &(gs_p->players[turn]);
    int l_i = p_p->location_index;

    Location_t * l_p = &(locations[l_i]);
    int l_type = l_p->location_type;

    int assets, ten_percent, least_amount;
    int card;

    switch(l_type) {
        case GO_T:
            /* NOTE: advance_token gives the money; don't do it again here */
            break;
        case PROPERTY_T:
            land_on_property_action(gs_p,p_p,l_i);
            break;
        case CHEST_T:
            card = rand() % num_chance_cards;
            printf("%s\n",community_chest_cards[card].text);
            break;
        case CHANCE_T:
            card = rand() % num_chance_cards;
            printf("%s\n",community_chest_cards[card].text);
            break;
        case INCOME_TAX_T:
            printf("You may pay either 10%% or $200\n");
            assets = calculate_assets(p_p);
            ten_percent = assets/10;
            least_amount = min(200,ten_percent);
            credit_or_debit_player(p_p, -1*least_amount);
            break;
        case LUXURY_TAX_T:
            credit_or_debit_player(p_p, -75);
            break;
        case JAIL_T:
            /* just visiting */
            break;
        case GO_TO_JAIL_T:
            enter_jail(p_p);
            break;
        case PARKING_T:
            /* collect money? */
            break;
    }

}

/* function summary:
       handle "in jail" case
       roll dice, handle going to jail for 3 doubles
       advance token
       do location action
       advance turn
*/

#define TRADE        0
#define MANAGE       1
#define ROLL         2
#define BUY          3
#define AUCTION      4
#define RENT         5
#define END          6
#define JAIL_ROLL    7
#define BAIL         8
#define GET_OUT_FREE 9

#define NUM_ACTION_STRINGS  10
int actions_allowed[NUM_ACTION_STRINGS];
char action_strings[][40] = {
    "Offer trade",
    "Manage property",
    "Roll dice",
    "Buy property",
    "Start auction",
    "Pay rent",
    "End turn",
    "Roll to get out of jail",
    "Pay to get out of jail",
    "Use get-out-of-jail-free card"
};

void game_iter(Game_state_t * gs_p) {

    int turn = gs_p->turn;
    Player_t * p_p = &(gs_p->players[turn]);

    int in_jail = p_p->in_jail;
    int end_of_turn = 0;
    int has_rolled = 0;
    int landed_on_unowned = 0;
    int doubles;
    char buffer[MAXCHARS];
    int i;

    /* let the player choose what to do */

    /* TODO: when player rolls doubles, they don't really "end turn" */

    while(!end_of_turn) {
        int choice;
        int die1, die2;
        int l_i;
        Location_t * l_p;
        int price, sufficient_funds;

        if(in_jail) {
            actions_allowed[TRADE]          = 1;
            actions_allowed[MANAGE]         = 1;
            actions_allowed[ROLL]           = 0;
            actions_allowed[BUY]            = 0;
            actions_allowed[AUCTION]        = 0;
            actions_allowed[RENT]           = 0;
            actions_allowed[END]            = has_rolled ? 1 : 0;
            actions_allowed[JAIL_ROLL]      = has_rolled ? 0 : 1;
            actions_allowed[BAIL]           = has_rolled ? 0 : 1;
            actions_allowed[GET_OUT_FREE]   = has_rolled ? 0 : 1;
        }
        else {
            actions_allowed[TRADE]          = 1;
            actions_allowed[MANAGE]         = 1;
            actions_allowed[ROLL]           = has_rolled ? 0 : 1;
            actions_allowed[BUY]            = landed_on_unowned ? 1 : 0;
            actions_allowed[AUCTION]        = landed_on_unowned ? 1 : 0;
            actions_allowed[RENT]           = p_p->rent_left_to_pay > 0 ? 1 : 0;
            actions_allowed[END]            = (has_rolled && !landed_on_unowned && p_p->rent_left_to_pay == 0) ? 1 : 0;
            actions_allowed[JAIL_ROLL]      = 0;
            actions_allowed[BAIL]           = 0;
            actions_allowed[GET_OUT_FREE]   = 0;
        }

ask_for_choice:
        printf("\n\n%s\n",equals);
        printf("What would you like to do?\n");
        printf("%s\n",equals);

        for(i=0; i < NUM_ACTION_STRINGS; i++) {
            if(actions_allowed[i])
                printf("%d. %s\n",i,action_strings[i]);
        }

        printf("%s: ",p_p->name);
        fgets(buffer,MAXCHARS,stdin);
        printf("\n\n\n");
        choice = atoi(buffer);

        if(choice < 0 || choice > NUM_ACTION_STRINGS || !actions_allowed[choice]) {
            printf("invalid choice\n");
            goto ask_for_choice;
        }

        l_i = p_p->location_index;
        l_p = &(locations[l_i]);
        price = l_p->price;

        switch(choice) {
            case TRADE:
                offer_trade(gs_p, p_p);
                break;
            case MANAGE:
                manage_property(gs_p,p_p);
                break;
            case ROLL:
                do_roll(gs_p,&doubles,&landed_on_unowned);
                has_rolled = 1;
                if(p_p->in_jail)
                    end_of_turn = 1;
                break;
            case BUY:
                sufficient_funds = (p_p->cash >= price);
                if(sufficient_funds) {
                    credit_or_debit_player(p_p, -1*price);
                    make_player_owner(p_p, l_i);
                    landed_on_unowned = 0;
                }
                else {
                    printf("INSUFFICIENT FUNDS TO MAKE PURCHASE\n");
                }
                break;
            case AUCTION:
                do_auction(gs_p, l_i);
                landed_on_unowned = 0;
                break;
            case RENT:
                charge_rent(gs_p, p_p, l_p);
                break;
            case END:
                end_of_turn = 1;
                break;
            case JAIL_ROLL:
                roll_dice(&die1, &die2);
                doubles = (die1 == die2);
                if(doubles) {
                    p_p->in_jail = 0;
                }
                has_rolled = 1;
                break;
            case BAIL:
                credit_or_debit_player(p_p, -50);
                p_p->in_jail = 0;
                break;
            case GET_OUT_FREE:
                p_p->in_jail = 0;
                p_p->num_get_out_of_jail_free--;
                break;
        }

    }

    printf("\n\n\n\n\n\n");
    advance_turn(gs_p, doubles);
}

/* TODO*/
void manage_property(Game_state_t * gs_p, Player_t * p_p) {
    char buffer[MAXCHARS];

    enum manage_choice_e {
        BUY_M,
        SELL_M,
        MORTGAGE_M,
        UNMORTGAGE_M,
        FINISH_M
    };

    int choice = 0;

    do {
        int i;

        printf("%s\n",equals);
        printf("MANAGE PROPERTY MENU\n");
        printf("%s\n",equals);
        printf("%d. buy houses or hotels\n", BUY_M);
        printf("%d. sells houses or hotels\n", SELL_M);
        printf("%d. mortgage property\n", MORTGAGE_M);
        printf("%d. unmortgage property\n", UNMORTGAGE_M);
        printf("%d. finished\n", FINISH_M);

        fgets(buffer,MAXCHARS,stdin);
        choice = atoi(buffer);

        switch(choice) {
            case BUY_M:
                printf("you own the following monopolies:\n");
                for(i=0; i < NUM_PROPERTY_GROUPS; i++) {
                    /* TODO: make this not fucking ugly*/
                    if(monopoly_owner(locations+property_groups[i][1]) == p_p)
                        ;
                }
                break;
            case SELL_M:
                printf("you own the following houses and hotels:\n");
                /* TODO*/
                break;
            case MORTGAGE_M:
                printf("you own the following unmortgaged properties:\n");
                /* TODO*/
                break;
            case UNMORTGAGE_M:
                printf("you own the following mortgaged properties:\n");
                /* TODO*/
                break;
            case FINISH_M:
                break;
            default:
                printf("invalid input\n");
                break;
        }
    } while(choice != FINISH_M);
}

void do_auction(Game_state_t * gs_p, int l_i) {

    int num_participants = gs_p->num_players;
    char buffer[MAXCHARS];
    int current_bid = 0; /* bid starts at 0 */
    int bidder = 0; /* index of current player making a bid */
    Player_t * highest_bidder_p;

    /* if everyone passes initially, goes to last player by default */
    int highest_bidder = gs_p->num_players-1;

    /* assume everyone wants to participate on the first round */
    int * participants = (int *) malloc(sizeof(int)*gs_p->num_players);
    int i;
    for(i=0; i < gs_p->num_players; i++)
        participants[i] = 1;

    /* auction is over once all but one player have dropped out */
    while(num_participants > 1) {

        int bid, choice;
        char * bidder_name = gs_p->players[bidder].name;
        int cash;
        int exceeds_current;
        int sufficient_funds;
        int valid_bid;

        /* skip players who have dropped out of bid */
        if(!participants[bidder])
            continue;

        printf("%s's turn to bid\n", bidder_name);

        /* prompt player to bid and ask how much */

        do {
            printf("the current bid is %d. would you like to bid? ",current_bid);
            fgets(buffer,MAXCHARS,stdin);
            choice = buffer[0];
        } while(buffer[0] != 'n' && buffer[0] != 'y');

        if(choice != 'y') {
            participants[bidder] = 0;
            num_participants--;
            continue;
        }

        printf("enter bid: ");
        fgets(buffer,MAXCHARS,stdin);
        bid = atoi(buffer);

        /* check if bid is valid; if so, update current bid */

        cash = gs_p->players[bidder].cash; /* should this be total assets?*/
        exceeds_current = (bid > current_bid);
        sufficient_funds = (bid <= cash);
        valid_bid = exceeds_current && sufficient_funds;

        if(valid_bid) {
            current_bid = bid;
            highest_bidder = bidder;
            bidder = (bidder+1) % gs_p->num_players;
        }
        else if(!exceeds_current) {
            printf("you must bid more than %d\n",current_bid);
        }
        else if(!sufficient_funds) {
            printf("YOU DO NOT HAVE SUFFICIENT FUNDS TO MAKE SUCH A BID\n");
        }
    }

    highest_bidder_p = &(gs_p->players[highest_bidder]);
    printf("sold! to %s for %d\n", highest_bidder_p->name, current_bid);

    credit_or_debit_player(highest_bidder_p, -1*current_bid);
    make_player_owner(highest_bidder_p, l_i);
}

void make_player_owner(Player_t * p_p, int l_i) {
    Location_t * l_p = &(locations[l_i]);

    /* take away from old owner, if there is one */
    if(l_p->ownership.owned) {
        Player_t * old_owner = l_p->owner;
        old_owner->property[l_i].own = 0;
        /* TODO: create assertion for 0 houses*/
    }

    /* and give to the new owner */
    p_p->property[l_i].own = 1;
    l_p->ownership.owned = 1;
    l_p->owner = p_p;
    printf("%s, you now own %s\n", p_p->name, l_p->name);
    if(monopoly_owner(l_p) == p_p)
        printf("you now own a monopoly!\n");
}

void enter_jail(Player_t * p_p) {
    p_p->in_jail = 1;
    p_p->rolls_in_jail = 0;
    p_p->roll_double_count = 0;
}

void do_roll(Game_state_t * gs_p, int * doubles, int * landed_on_unowned) {

    int turn = gs_p->turn;
    Player_t * p_p = &(gs_p->players[turn]);
    Location_t * l_p;

    /* roll dice */
    int die1, die2;
    roll_dice(&die1, &die2);

    *doubles = (die1 == die2);

    printf("die 1: %d\ndie 2: %d\n",die1,die2);

    if(*doubles) {
        printf("YOU ROLLED DOUBLES\n");
        p_p->roll_double_count++;
    }
    else {
        p_p->roll_double_count = 0;
    }

    /* if roll double count is 3, go to jail, end turn */
    if(p_p->roll_double_count == 3) {
        printf("you rolled doubles three times in a row. you go to jail.\n");
        enter_jail(p_p);
        return;
    }

    /* advance token; handles rolling doubles */
    advance_token(p_p,die1,die2);

    l_p = &(locations[p_p->location_index]);
    *landed_on_unowned = (l_p->location_type == PROPERTY_T) && (!l_p->ownership.owned);

    do_location_action(gs_p);
}

/* TODO*/
void offer_trade(Game_state_t * gs_p, Player_t * p_p) {

    char buffer[MAXCHARS];
    int partner_choice;
    int i;
    int choice;
    int ready_to_trade;

    enum trade_choice_e {
        CASH_O, PROPERTY_O, JAIL_FREE_O,
        CASH_R, PROPERTY_R, JAIL_FREE_R,
        FINISH_T,
        CANCEL_T};

    /* initialize trade details */
    Trade_t trade_details;
    trade_details.player_making_offer = p_p;
    trade_details.partner = NULL; /* default */
    trade_details.cash_offer = 0;
    trade_details.cash_request = 0;
    trade_details.get_out_free_card_offer = 0;
    trade_details.get_out_free_card_request = 0;

    for(i=0; i<NUM_LOC; i++) {
        trade_details.properties_offer[i] = 0;
        trade_details.properties_request[i] = 0;
    }

    /* if there are only two players, the partner must be the other player */
    if(gs_p->num_players == 2) {
        if((&gs_p->players[0]) == p_p)
            trade_details.partner = &gs_p->players[1];
        else
            trade_details.partner = &gs_p->players[0];
    }
    else {
prompt_for_partner:
        printf("please choose a partner with whom to trade\n");
        for(i=0; i<gs_p->num_players; i++)
            printf("%d. %s\n",i,gs_p->players[i].name);
        fgets(buffer, MAXCHARS, stdin);
        partner_choice = atoi(buffer);

        if(partner_choice < 0 || (partner_choice > (gs_p->num_players-1))) {
            printf("%d: invalid input\n", partner_choice);
            printf("%d\n", gs_p->num_players-1);
            goto prompt_for_partner;
        }

        trade_details.partner = &(gs_p->players[partner_choice]);
    }

    choice = 0;
    ready_to_trade = 0;

    while(!ready_to_trade) {
        int cash_offer;
        int cash_request;
        int sufficient_funds;

        printf("TRADE MENU\n");
        printf("%s\n",equals);
        printf("%d. offer cash amount (currently %d)\n", CASH_O, trade_details.cash_offer);
        printf("%d. offer properties\n", PROPERTY_O);
        if(p_p->num_get_out_of_jail_free > 0)
            printf("%d. offer get-out-of-jail-free card\n", JAIL_FREE_O);
        printf("%d. request cash amount (currently %d)\n", CASH_R, trade_details.cash_request);
        printf("%d. request properties\n", PROPERTY_R);
        if(trade_details.partner->num_get_out_of_jail_free > 0)
            printf("%d. request get-out-of-jail-free card\n", JAIL_FREE_R);
        printf("%d. finished; extend offer\n", FINISH_T);
        printf("%d. cancel trade\n", CANCEL_T);
        printf("%s\n",equals);
        printf("choice: ");

        fgets(buffer,MAXCHARS,stdin);
        choice = atoi(buffer);
        if(choice < 0 || choice > CANCEL_T) {
            printf("invalid choice\n");
            continue;
        }

        switch(choice) {
            case CASH_O:
                printf("Cash amount to offer: ");
                fgets(buffer,MAXCHARS,stdin);
                cash_offer = atoi(buffer);
                sufficient_funds = (cash_offer <= p_p->cash);
                if(sufficient_funds)
                    trade_details.cash_offer = cash_offer;
                else
                    printf("YOU HAVE INSUFFICIENT FUNDS TO MAKE THAT OFFER\n");
                break;
            case PROPERTY_O:
                printf("You may offer any of the following properties:\n");
                printf("(enter number to toggle property from offer)\n");
                make_property_trade_list(p_p, &(trade_details.properties_offer[0]));
            case JAIL_FREE_O:
                if(p_p->num_get_out_of_jail_free > 0)
                    trade_details.get_out_free_card_offer = !trade_details.get_out_free_card_offer;
                else
                    printf("you do not have a get-out-of-jail free card\n");
                break;
            case CASH_R:
                printf("cash amount to request: ");
                fgets(buffer,MAXCHARS,stdin);
                cash_request = atoi(buffer);
                /* TODO: make sure player has sufficient funds*/
                trade_details.cash_request = cash_request;
                break;
            case PROPERTY_R:
                printf("You may request any of the following properties:\n");
                printf("(enter number to toggle property from offer)\n");
                make_property_trade_list(trade_details.partner, &(trade_details.properties_request[0]));
                break;
            case JAIL_FREE_R:
                printf("feature not yet available\n");
                trade_details.get_out_free_card_request = !trade_details.get_out_free_card_request;
                break;
            case FINISH_T:

                /* TODO: make this not fucking ugly*/

                printf("%s\nTrade details:\n%s\n",equals,equals);
                printf("                  You give:                 They give:\n");
                printf("Cash:             %d                         %d\n",trade_details.cash_offer, trade_details.cash_request);
                printf("Property:\n");
                for(i=0; i<NUM_LOC; i++) {
                    if(trade_details.properties_offer[i])
                        printf("                  %s\n",locations[i].name);
                    if(trade_details.properties_request[i])
                        printf("                                            %s\n",locations[i].name);
                }
                    printf("Get-out-of-jail\nfree card:\n");
                if(trade_details.get_out_free_card_offer)
                    printf("                  X\n");
                if(trade_details.get_out_free_card_request)
                    printf("                                            X\n");
                printf("\nARE YOU SURE YOU WISH TO MAKE THIS OFFER? (type 'yes' to confirm)\n");
                fgets(buffer,MAXCHARS,stdin);
                if(strncmp(buffer,"yes\n",4) == 0)
                    ready_to_trade = 1;
                else
                    printf("trade not confirmed\n");
                break;
            case CANCEL_T:
                printf("trade cancelled...\n");
                ready_to_trade = 1;
                break;
        }
    }

    if(choice == CANCEL_T)
        return;

    printf("%s, would you like to accept this offer? (type 'yes' to accept)\n", trade_details.partner->name);
    fgets(buffer,MAXCHARS,stdin);
    if(strncmp(buffer,"yes\n",4) != 0) {
        printf("trade offer denied\n");
        return;
    }

    /* TODO: execute trade*/
    printf("executing trade...\n");

    /* exchange cash */
    if(trade_details.cash_offer > 0) {
        credit_or_debit_player(trade_details.partner,                trade_details.cash_offer);
        credit_or_debit_player(trade_details.player_making_offer, -1*trade_details.cash_offer);
    }
    if(trade_details.cash_request > 0) {
        credit_or_debit_player(trade_details.partner,             -1*trade_details.cash_offer);
        credit_or_debit_player(trade_details.player_making_offer,    trade_details.cash_offer);
    }

    /* exchange properties */
    for(i=0; i<NUM_LOC; i++) {
        if(trade_details.properties_offer[i])
            make_player_owner(trade_details.partner, i);
        if(trade_details.properties_request[i])
            make_player_owner(trade_details.player_making_offer, i);
    }

    /* TODO: get-out-of-jail-free card exchange */
}

void make_property_trade_list(Player_t * p_p, int * list) {

    int i;
    int prop_index = 0;
    char buffer[MAXCHARS];

    while(prop_index != NUM_LOC) {

        for(i=0; i<NUM_LOC; i++) {
            if(p_p->property[i].own && p_p->property[i].num_houses == 0) {
                printf("%d. ",i);
                if(list[i])
                    printf("X");
                else
                    printf(" ");
                printf(" %s\n",locations[i].name);
            }
        }
        printf("%d. done\n",NUM_LOC);
        fgets(buffer,MAXCHARS,stdin);
        prop_index = atoi(buffer);
        if(prop_index > 0 && prop_index < NUM_LOC) {
            if(p_p->property[prop_index].own)
                list[prop_index] = !list[prop_index];
            else
                printf("that property cannot be chosen for trade\n");
        }
        else if(prop_index == NUM_LOC)
            break;
        else
            printf("invalid input\n");
    }
}


void game_loop(Game_state_t * gs_p) {

    while(!gs_p->game_over) {
        game_iter(gs_p);
    }

}

void end_game(Game_state_t * gs_p) {
    /* TODO: free allocated resources*/
}
