#include "monopoly.h"

/*
   Questions:

 * What happens when you enter bankrupty while owing to multiple people?
   How are your assets divided?

 * When you are in jail and roll dice in an attempt to get doubles, your doubles
   do not allow you to roll again. However, if you roll two doubles on your next turn,
   do you go to jail? In other words, should the doubles to get you out of jail
   increment the roll_double_count?

*/

/* major TODO items:
 * verify that liquid and taxable assets are being calculated correctly
 * consider calculating liquid and taxable assets on the fly vs. through book-keeping
 * chance and community chest cards
 * finish trade functionality
 * allow players to manage property
 * bankruptcy
 */

/*
Notes:
   ways to raise money:
   * offer a trade
   * sell houses and hotels
   * mortgage a property

   income tax:
   * cash
   * original price of properties and houses and hotels

   assets:
   * cash
   * mortgage value of property
   * houses and hotels sell at half price
   * 
*/

/* DEBUG modes: */
/*#define DEBUG_MASTER */
/*#define DEBUG_SPLIT */
/*#define DEBUG_GOOJFC */
/*#define DEBUG_DOUBLES */
/*#define DEBUG_LOW_CASH */
/*#define DEBUG_CARDS */

#define min(x,y) (x) < (y) ? (x) : (y)

const char equals[] = "================================================";

char buffer[MAXCHARS];

/* first number is the number of properties in the group */
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
    {"Reading Railroad",     PROPERTY_T,   RAIL,    200,   0,   0,100, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Oriental Avenue",      PROPERTY_T,   GRAY,    100,  50,  50, 50, { 6, 30, 90, 270, 400, 550}, {0, 0, 0, 0}, NULL},
    {"Chance",               CHANCE_T,     NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Vermont Avenue",       PROPERTY_T,   GRAY,    100,  50,  50, 50, { 6, 30, 90, 270, 400, 550}, {0, 0, 0, 0}, NULL},
    {"Connecticut Avenue",   PROPERTY_T,   GRAY,    120,  50,  50, 60, { 8, 40,100, 300, 450, 600}, {0, 0, 0, 0}, NULL},
    {"Jail",                 JAIL_T,       NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"St. Charles Place",    PROPERTY_T,   PINK,    140, 100, 100, 70, {10, 50,150, 450, 625, 750}, {0, 0, 0, 0}, NULL},
    {"Electric Company",     PROPERTY_T,   UTILITY, 150,   0,   0, 75, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"States Avenue",        PROPERTY_T,   PINK,    140, 100, 100, 70, {10, 50,150, 450, 625, 750}, {0, 0, 0, 0}, NULL},
    {"Virginia Avenue",      PROPERTY_T,   PINK,    160, 100, 100, 80, {12, 60,180, 500, 700, 900}, {0, 0, 0, 0}, NULL},
    {"Pennsylvania Railroad",PROPERTY_T,   RAIL,    200,   0,   0,100, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"St. James Place",      PROPERTY_T,   ORANGE,  180, 100, 100, 90, {14, 70,200, 550, 750, 950}, {0, 0, 0, 0}, NULL},
    {"Community Chest",      CHEST_T,      NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Tennessee Avenue",     PROPERTY_T,   ORANGE,  180, 100, 100, 90, {14, 70,200, 550, 750, 950}, {0, 0, 0, 0}, NULL},
    {"New York Avenue",      PROPERTY_T,   ORANGE,  200, 100, 100,100, {16, 80,220, 600, 800,1000}, {0, 0, 0, 0}, NULL},
    {"Free Parking",         PARKING_T,    NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Kentucky Avenue",      PROPERTY_T,   RED,     220, 150, 150,110, {18, 90,250, 700, 875,1050}, {0, 0, 0, 0}, NULL},
    {"Chance",               CHANCE_T,     NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Indiana Avenue",       PROPERTY_T,   RED,     220, 150, 150,110, {18, 90,250, 700, 875,1050}, {0, 0, 0, 0}, NULL},
    {"Illinois Avenue",      PROPERTY_T,   RED,     240, 150, 150,120, {20,100,400, 750, 925,1100}, {0, 0, 0, 0}, NULL},
    {"B.&O. Railroad",       PROPERTY_T,   RAIL,    200,   0,   0,100, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Atlantic Avenue",      PROPERTY_T,   YELLOW,  260, 150, 150,130, {22,110,330, 800, 975,1150}, {0, 0, 0, 0}, NULL},
    {"Ventnor Avenue",       PROPERTY_T,   YELLOW,  260, 150, 150,130, {22,110,330, 800, 975,1150}, {0, 0, 0, 0}, NULL},
    {"Water Works",          PROPERTY_T,   UTILITY, 150,   0,   0, 75, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Marvin Gardens",       PROPERTY_T,   YELLOW,  280, 150, 150,140, {24,120,360, 850,1025,1150}, {0, 0, 0, 0}, NULL},
    {"GO TO JAIL",           GO_TO_JAIL_T, NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Pacific Avenue",       PROPERTY_T,   GREEN,   300, 200, 200,150, {26,130,390, 900,1100,1275}, {0, 0, 0, 0}, NULL},
    {"North Carolina Avenue",PROPERTY_T,   GREEN,   300, 200, 200,150, {26,130,390, 900,1100,1275}, {0, 0, 0, 0}, NULL},
    {"Community Chest",      CHEST_T,      NONE,      0,   0,   0,  0, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
    {"Pennsylvania Avenue",  PROPERTY_T,   GREEN,   320, 200, 200,160, {28,150,450,1000,1200,1400}, {0, 0, 0, 0}, NULL},
    {"Short Line",           PROPERTY_T,   RAIL,    200,   0,   0,100, { 0,  0,  0,   0,   0,   0}, {0, 0, 0, 0}, NULL},
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
    int p_i;
    int num_players;

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
    } while(num_players < 2 || num_players > MAX_PLAYERS);

    gs_p->num_players = num_players;
    gs_p->num_active_players = num_players; /* nobody is bankrupt */
    gs_p->players = malloc(sizeof(Player_t)*num_players); /* create players */
    gs_p->turn.debts = malloc(sizeof(int)*(num_players+1)); /* +1 for the bank */

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

        p_p->index = i; /* TODO: should this stay? only used by indebt_current_player */

/*        printf("piece type: ");
        fgets(buffer,MAXCHARS,stdin);
        chomp(buffer); */
        p_p->piece_type = 0; /* TODO */

#ifdef DEBUG_LOW_CASH
        p_p->cash = 10;
#else
        p_p->cash = 1500;
#endif
        p_p->liquid_assets = p_p->cash;
        p_p->taxable_assets = p_p->cash; /* perhaps this should always be calculated on the fly */
        p_p->location_index = GO_L;
        p_p->in_jail = 0;

        p_p->bankrupt = 0;

        for(j=0; j<NUM_LOC; j++) /* could use memset... */
            p_p->property[i] = 0;
#ifdef DEBUG_GOOJFC
        p_p->num_get_out_of_jail_free = 2;
#else
        p_p->num_get_out_of_jail_free = 0;
#endif

    }

    /* FOR TESTING: initially give all property to player 0 */
#ifdef DEBUG_MASTER
    {
    Player_t * master = &(gs_p->players[0]);
    for(i=0; i < NUM_LOC; i++) {
        if(locations[i].location_type == PROPERTY_T)
            make_player_owner(master, i);
    }
    }
#else
#ifdef DEBUG_SPLIT
    {
    Player_t * p0 = &(gs_p->players[0]);
    Player_t * p1 = &(gs_p->players[1]);
    for(i=0; i < NUM_LOC/2; i++) {
        if(locations[i].location_type == PROPERTY_T)
            make_player_owner(p0, i);
    }
    for(i=NUM_LOC/2; i < NUM_LOC; i++) {
        if(locations[i].location_type == PROPERTY_T)
            make_player_owner(p1, i);
    }
    }
#endif
#endif

    /* choose random starting player */
    p_i = gs_p->turn.player_index = rand() % num_players;
    printf("\nRandom starting player is %s\n",gs_p->players[p_i].name);
    pause();
}

void pause() {
    printf("[press enter to continue]\n");
    fgets(buffer,MAXCHARS,stdin);
}

void advance_turn(Game_state_t * gs_p) {
    int * p_i = &(gs_p->turn.player_index);
    int num_players = gs_p->num_players;
    Player_t * p_p;

next_player:
    *p_i = (*p_i + 1) % num_players;
    p_p = &(gs_p->players[*p_i]);
    if(p_p->bankrupt) /* skip over bankrupt players */
        goto next_player;

    /* Used to print game state... */
}

/* NOTE: Should only be called when the player has sufficient cash to
         pay off all debts. */
void pay_off_debts(Game_state_t * gs_p) {
    int i;
    int player_index = gs_p->turn.player_index;
    Player_t * debtor = &(gs_p->players[player_index]);

    for(i=0; i < gs_p->num_players; i++) {
        Player_t * creditor = &(gs_p->players[i]);
        int debt = gs_p->turn.debts[i];
        if(debt == 0)
            continue;
        credit_player(creditor, debt);
        debit_player(debtor, debt);
        gs_p->turn.debts[i] = 0;
        gs_p->turn.debt_total -= debt;
    }

    if(gs_p->turn.debts[i] > 0)
        debit_player(debtor, gs_p->turn.debts[i]);
    /* no need to credit the bank */

    gs_p->turn.in_debt = 0;
}

void indebt_current_player(Turn_t * t, int creditor, int amount) {
    t->in_debt = 1;
    t->debt_total += amount;
    t->debts[creditor] += amount;
}

void credit_player(Player_t * p_p, int amount) {
    char * p_name = p_p->name;
    p_p->cash += amount;
    printf("%s, your account has been credited $%d. ", p_name, amount);
    printf("You now have $%d.\n", p_p->cash);
}

/* TODO: assertion for sufficient funds; should never be used when player does not have suffucient funds */
void debit_player(Player_t * p_p, int amount) {
    char * p_name = p_p->name;

    /* if insufficient funds */
    if(p_p->cash < amount) {
        printf("%s, YOU DO NOT HAVE SUFFICIENT FUNDS TO PAY $%d.\n", p_name, amount);
        printf("You must raise money by offering a trade or by selling or mortgaging property\n");
        pause();
    }
    else {
        p_p->cash -= amount;
        printf("%s, your account has been debited $%d. ", p_name, amount);
        printf("You now have $%d.\n", p_p->cash);
        pause();
    }
}

void advance_token(Player_t * p_p, int die1, int die2) {

    int * l_i = &(p_p->location_index);
    int sum = die1 + die2;
    int new_l_i = (*l_i+sum) % NUM_LOC;
    int i;

#ifdef DEBUG_CARDS
    static int card_index = 0;
    int card_indices[] = {COM0_L, COM1_L, COM2_L, CHA0_L, CHA1_L, CHA2_L};
    *l_i = card_indices[card_index];
    card_index++;
    return;
#endif

    for(i=1; i<sum+1; i++) {
        int loc = (*l_i+i) % NUM_LOC;
        if(i==sum)
            printf("YOU LANDED ON: --->%s<---\n",locations[loc].name);
        else
            printf("                   %s\n",locations[loc].name);
        if(loc == GO_L) {
            printf("You passed GO! Collect $200!\n");
            credit_player(p_p,200);
        }
    }
    printf("\n");

    *l_i = new_l_i;
}

void roll_dice(Game_state_t * gs_p) {
#ifdef DEBUG_DOUBLES
    int die1 = gs_p->turn.die1 = 2;
    int die2 = gs_p->turn.die2 = 2;
#else
    int die1 = gs_p->turn.die1 = (rand() % 6)+1;
    int die2 = gs_p->turn.die2 = (rand() % 6)+1;
#endif
    gs_p->turn.doubles = (die1 == die2);
    gs_p->turn.has_rolled = 1;
    printf("die 1: %d\ndie 2: %d\n",die1,die2);
}

int calculate_rent(Game_state_t * gs_p, Location_t * l_p) {
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
        int number_owned = 0;
        int i;
        Player_t * owner = l_p->owner;
        for(i=1; i < property_groups[RAIL][0]+1; i++){
            int l_i = property_groups[RAIL][i];
            if(locations[l_i].owner == owner)
                number_owned++;
        }
        switch(number_owned) {
            case 1: rent = 25; break;
            case 2: rent = 50; break;
            case 3: rent = 100; break;
            case 4: rent = 200; break;
            default: printf("error: contradiction in railroad code\n"); break;
        }
    }
    else if(l_p->group == UTILITY) {
        /* TODO: rent depends on how many are owned */
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

void charge_rent(Game_state_t * gs_p, Location_t * l_p) {

    Player_t * owner = l_p->owner;

    int rent = calculate_rent(gs_p, l_p);
    printf("rent is: $%d\n",rent);

    indebt_current_player(&(gs_p->turn), owner->index, rent);
}

/* TODO: might be able to remove the p_p parameter */
void land_on_property_action(Game_state_t * gs_p, Player_t * p_p, int l_i) {
    Location_t * l_p = &(locations[l_i]);
    int owned = l_p->ownership.owned;
    if(owned) {
        Player_t * owner = l_p->owner;
        int owned_by_another = (owner == p_p) ? 0 : 1;
        if(owned_by_another) {
            printf("Property is owned by another player.\n");
            charge_rent(gs_p, l_p);
        }
        else
            printf("(You own this property)\n");
    }
    else {
        printf("\nYou have landed on an unowned property.\nYou must buy it or it will be auctioned.\n");
    }

}

void do_location_action(Game_state_t * gs_p) {

    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);
    int l_i = p_p->location_index;

    Location_t * l_p = &(locations[l_i]);
    int l_type = l_p->location_type;

    int bank = gs_p->num_players;

    int ten_percent, least_amount;
    int card;

    switch(l_type) {
        case GO_T:
            /* NOTE: advance_token gives the money; don't do it again here */
            break;
        case PROPERTY_T:
            land_on_property_action(gs_p,p_p,l_i);
            break;
        case CHEST_T:
            card = rand() % num_comm_chest_cards;
            printf("%s\n",community_chest_cards[card].text);
            community_chest_cards[card].func(gs_p);
            break;
        case CHANCE_T:
            card = rand() % num_chance_cards;
            printf("%s\n",chance_cards[card].text);
            chance_cards[card].func(gs_p);
            break;
        case INCOME_TAX_T:
            printf("You may pay either 10%% or $200\n");
            ten_percent = p_p->taxable_assets/10;
            least_amount = min(200,ten_percent);
            indebt_current_player( &(gs_p->turn), bank, least_amount);
            break;
        case LUXURY_TAX_T:
            indebt_current_player( &(gs_p->turn), bank, 75);
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

int actions_allowed[NUM_ACTION_STRINGS];
char action_strings[][40] = {
    "Offer trade",
    "Manage property",
    "Roll dice",
    "Buy property",
    "Start auction",
    "Pay debts",
    "End turn",
    "Roll to get out of jail",
    "Pay to get out of jail",
    "Use get-out-of-jail-free card"
};

void print_game_state(Game_state_t * gs_p) {

    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);
    char * p_name = p_p->name;
    int i;
    int rdc = gs_p->turn.roll_double_count;
    int name_len;
    char last_char;

    printf("%s\n",equals);
    name_len = strlen(p_name);
    last_char = p_name[name_len-1];
    if(last_char == 's')
        printf("It is now %s' turn\n",p_name);
    else
        printf("It is now %s's turn\n",p_name);
    printf("%s\n",equals);

    if(p_p->in_jail) {
        printf("You are currently in jail.\n");
        printf("You have tried to roll for doubles %d times.\n", p_p->rolls_in_jail);
    }
    if(p_p->num_get_out_of_jail_free)
        printf("You have %d get-out-of-jail-free cards.\n",p_p->num_get_out_of_jail_free);
    if(gs_p->turn.has_rolled)
        printf("You have rolled.\n");
    if(rdc > 0)
        printf("You have rolled doubles %d times consecutively.\n", rdc);
    if(gs_p->turn.in_debt)
        printf("You are $%d in debt.\n",gs_p->turn.debt_total);
    if(gs_p->turn.on_unowned)
        printf("You are currently on an unowned property.\n");

    printf("Assets:\n");
    printf("$%d cash on hand\n",p_p->cash);
    printf("Properties: \n");

    for(i=0; i<NUM_LOC; i++) {
        if(p_p->property[i]) {
            int num_houses = locations[i].ownership.num_houses;
            printf("    %s (MV=$%d)",locations[i].name,locations[i].mortgage_value);
            if(locations[i].ownership.mortgaged)
                printf("[MORTGAGED]\n");
            else if(num_houses > 0)
                printf("with %d houses\n", num_houses);
            else if(locations[i].ownership.hotel)
                printf("with a hotel\n");
            else
                printf("\n");
        }
    }

    printf("You total liquid assets sum to: $%d\n",p_p->liquid_assets);
}

void game_iter(Game_state_t * gs_p) {

    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);


    int i;
    int end_of_turn = 0;
    /* initialize turn data */
    gs_p->turn.has_rolled = 0;
    gs_p->turn.on_unowned = 0;
    gs_p->turn.roll_double_count = 0;
    gs_p->turn.in_debt = 0;
    gs_p->turn.debt_total = 0;
    gs_p->turn.doubles = 0;

    /* at the beginning of the turn, the player does not owe anyone */
    for(i=0; i < gs_p->num_players+1; i++) /* +1 for bank */
        gs_p->turn.debts[i] = 0;

    /* let the player choose what to do */

    while(!end_of_turn) {
        int choice;
        int l_i;
        Location_t * l_p;
        int price, sufficient_funds;

        print_game_state(gs_p);

        if(p_p->liquid_assets < gs_p->turn.debt_total) {
            declare_bankruptcy(gs_p);
            end_of_turn = 1;
            continue;
        }

        if(p_p->in_jail) {
            actions_allowed[TRADE]          = 1;
            actions_allowed[MANAGE]         = 1;
            actions_allowed[ROLL]           = 0;
            actions_allowed[BUY]            = 0;
            actions_allowed[AUCTION]        = 0;
            actions_allowed[PAY_DEBT]       = 0;
            actions_allowed[END]            =  gs_p->turn.has_rolled ? 1 : 0;
            actions_allowed[JAIL_ROLL]      = !gs_p->turn.has_rolled ? 1 : 0;
            actions_allowed[BAIL]           = !gs_p->turn.has_rolled ? 1 : 0;
            actions_allowed[GET_OUT_FREE]   = (!gs_p->turn.has_rolled  && p_p->num_get_out_of_jail_free > 0) ? 1 : 0;
        }
        else {
            actions_allowed[TRADE]          = 1;
            actions_allowed[MANAGE]         = 1;

            /* TODO: this probably allows them to roll again after getting out of jail on doubles */
            /* if so, must differentiate between doubles that get you out of jail vs. doubles when not in jail */
            actions_allowed[ROLL]           = (!gs_p->turn.has_rolled || gs_p->turn.doubles) ? 1 : 0;
            actions_allowed[BUY]            = gs_p->turn.on_unowned ? 1 : 0;
            actions_allowed[AUCTION]        = gs_p->turn.on_unowned ? 1 : 0;
            actions_allowed[PAY_DEBT]       = gs_p->turn.in_debt ? 1 : 0;

            actions_allowed[END]            =
                ( gs_p->turn.has_rolled &&     /* must roll at least once */
                 !gs_p->turn.doubles    &&     /* if doubles, must roll again */
                 !gs_p->turn.on_unowned &&     /* if on unowned, must buy or auction */
                 !gs_p->turn.in_debt) ? 1 : 0; /* cannot end until paid off debts */

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
        choice = atoi(buffer);
        printf("\n\n\n");

        if(choice < 0 || choice > NUM_ACTION_STRINGS || !actions_allowed[choice]) {
            printf("\n\n\nINVALID CHOICE\n\n\n");
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
                do_roll(gs_p);
                if(p_p->in_jail) {
                    printf("TODO: make sure debts are paid before entering jail\n");
                    end_of_turn = 1;
/*                    attempt_pay_off_debts();
                    if(gs_p->turn.debt_total < p_p->cash) {
                        pay_off_debts(gs_p);
                         end_of_turn = 1;
                    }
                    else {
                        declare_bankruptcy();
                    } */
                }
                break;
            case BUY:
                sufficient_funds = (p_p->cash >= price);
                if(sufficient_funds) {
                    debit_player(p_p, price);
                    make_player_owner(p_p, l_i);
                    gs_p->turn.on_unowned = 0;
                }
                else {
                    printf("INSUFFICIENT FUNDS TO MAKE PURCHASE\n");
                    pause();
                }
                break;
            case AUCTION:
                do_auction(gs_p, l_i);
                gs_p->turn.on_unowned = 0; /* property will necessarily be owned after auction */
                break;
            case PAY_DEBT:
                if(gs_p->turn.debt_total < p_p->cash)
                    pay_off_debts(gs_p);
                else {
                    printf("YOU DO NOT HAVE ENOUGH CASH TO PAY YOUR DEBTS\n");
                    pause();
                }
                break;
            case END:
                end_of_turn = 1;
                break;
            case JAIL_ROLL:
                do_roll(gs_p);
                break;
            case BAIL:
                if(p_p->cash > BAIL_COST) {
                    debit_player(p_p, BAIL_COST);
                    p_p->in_jail = 0;
                }
                else {
                    printf("YOU CANNOT AFFORD BAIL\n");
                    pause();
                }
                break;
            case GET_OUT_FREE:
                p_p->in_jail = 0;
                p_p->num_get_out_of_jail_free--;
                break;
        }

    }

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    advance_turn(gs_p);
}

void declare_bankruptcy(Game_state_t * gs_p) {

    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);

    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("Your total liquid assets: %d\n",p_p->liquid_assets);
    printf("Total debt: %d\n",gs_p->turn.debt_total);
    printf("YOU ARE BANKRUPT!!!!\n");
    pause();

    p_p->bankrupt = 1;
    gs_p->num_active_players--;

    if(gs_p->num_active_players == 1) {
        int i;
        gs_p->game_over = 1;

        for(i = 0; i < gs_p->num_players; i++) {
            if(!gs_p->players[i].bankrupt) {
                gs_p->winner = &(gs_p->players[i]);
                break;
            }
        }
    }

    /* QUESTION: can you go bankrupt with debts to the bank as well as other players? */

    /* in any case: */
        /* must sell all houses and hotels */

    /* if you owe the bank: */
        /* must auction off all property */

    /* if you owe other player: */
        /* turn over all cash and titles to other player */

    /* if you owe multiple players... divide equally? */


}

void liquidate_assets() {

    /* sell all houses and hotels */

    /* */

}

void manage_property(Game_state_t * gs_p, Player_t * p_p) {

    enum manage_choice_e {
        BUY_M,
        SELL_M,
        MORTGAGE_M,
        UNMORTGAGE_M,
        FINISH_M
    };

    int choice = 0;

    do {
        printf("\n\n\n\n\n%s\n",equals);
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
            case BUY_M:        buy_houses(p_p);            break;
            case SELL_M:       sell_houses(p_p);           break;
            case MORTGAGE_M:   mortgage_properties(p_p);   break;
            case UNMORTGAGE_M: unmortgage_properties(p_p); break;
            case FINISH_M:                                 break;
            default: printf("\n\n\nINVALID INPUT\n\n\n");  break;
        }
    } while(choice != FINISH_M);
}

/* TODO: consider combining all of the manage property subroutines into a single routine */

void unmortgage_properties(Player_t * p_p) {
    int to_unmortgage;
    int valid_choices[NUM_LOC];
    int valid_choice;
    int i;
    int mortgage_value, cost, sufficient_funds;

    /* TODO: print "cost to unmortgage" somewhere for the player */

    while(to_unmortgage != NUM_LOC) {
        for(i=0; i<NUM_LOC; i++)
            valid_choices[i] = 0;

        printf("You own the following mortgaged properties:\n");
        for(i=0; i < NUM_LOC; i++) {
            if(p_p->property[i] && locations[i].ownership.mortgaged) {
                printf("%d. %s\n",i,locations[i].name);
                valid_choices[i] = 1;
            }
        }

        printf("%d. done\n",NUM_LOC);

        printf("Which property would you like to unmortgage: ");
        fgets(buffer,MAXCHARS,stdin);
        to_unmortgage = atoi(buffer);

        if(to_unmortgage == NUM_LOC)
            continue;

        valid_choice = (to_unmortgage >= 0 && to_unmortgage < NUM_LOC && valid_choices[to_unmortgage]);

        if(!valid_choice) {
            printf("\n\n\nINVALID CHOICE\n\n\n");
            continue;
        }

        mortgage_value = locations[to_unmortgage].mortgage_value;
        cost = mortgage_value * 1.1; /* 10% interest */
        sufficient_funds = p_p->cash >= cost;

        if(sufficient_funds) {
            debit_player(p_p, cost);
            locations[to_unmortgage].ownership.mortgaged = 0;
        }
        else {
            printf("YOU DO NO HAVE SUFFICIENT FUNDS TO UNMORTGAGE THAT PROPERTY\n");
            pause();
        }
            
    }
}

void mortgage_properties(Player_t * p_p) {

    int to_mortgage;
    int i;
    int valid_choices[NUM_LOC];
    int valid_choice;
    int mortgage_value;


    while(to_mortgage != NUM_LOC) {

        for(i=0; i<NUM_LOC; i++)
            valid_choices[i] = 0;

        printf("\n\n\n\n\n\n\n\n\n\n");

        printf("You own the following unmortgaged, unimproved properties:\n");

        for(i=0; i < NUM_LOC; i++) {

            if(p_p->property[i]                         &&
               !locations[i].ownership.mortgaged        &&
               (locations[i].ownership.num_houses == 0) &&
               !locations[i].ownership.hotel) {

                printf("%d. %s\n",i,locations[i].name);
                valid_choices[i] = 1;
            }
        }
        printf("%d. done\n",NUM_LOC);

        printf("Which property would you like to mortgage: ");
        fgets(buffer,MAXCHARS,stdin);
        to_mortgage = atoi(buffer);

        if(to_mortgage == NUM_LOC)
            continue;

        valid_choice = (to_mortgage >= 0 && to_mortgage < NUM_LOC && valid_choices[to_mortgage]);

        if(!valid_choice) {
            printf("\n\n\nINVALID CHOICE\n\n\n");
            continue;
        }

        mortgage_value = locations[to_mortgage].mortgage_value;
        credit_player(p_p, mortgage_value);
        locations[to_mortgage].ownership.mortgaged = 1;

        printf("%s has been mortgaged.\n",locations[to_mortgage].name);
        pause();
    }

}

/* TODO: allow players to sell hotels */
void sell_houses(Player_t * p_p) {

    int i, j;
    int to_sell;
    int valid_choices[NUM_LOC];
    int valid_choice;
    int house_price;

    for(i=0; i<NUM_LOC; i++)
        valid_choices[i] = 0;

    while(to_sell != NUM_LOC) {

        int max_houses = 0;
        printf("\n\n\n\n\n\n\n\n\n\n");

        printf("You own houses and hotels on the following properties:\n");

        for(i=0; i < NUM_LOC; i++) {
            int num_houses = locations[i].ownership.num_houses;

            if(num_houses == 0)
                continue;

            if(num_houses > max_houses)
                max_houses = num_houses;

            printf("%d. [",i);
            for(j=0; j < MAX_HOUSES; j++) {
                if(j < num_houses)
                    printf("X");
                else
                    printf(" ");
            }
            printf("] %s\n",locations[i].name);

            valid_choices[i] = 1;
        }

        printf("%d. done\n",NUM_LOC);

        printf("For which property would you like to sell houses or hotels: ");
        fgets(buffer,MAXCHARS,stdin);
        to_sell = atoi(buffer);

        if(to_sell == NUM_LOC)
            continue;

        valid_choice = (to_sell >= 0) && (to_sell < NUM_LOC) && valid_choices[to_sell];

        if(!valid_choice) {
            printf("\n\n\nINVALID CHOICE\n\n\n");
            continue;
        }

        if(locations[to_sell].ownership.num_houses == 0) {
            printf("\n\n\nTHIS PROPERTY HAS NO HOUSES TO SELL\n\n\n");
            pause();
            continue;
        }

        if(locations[to_sell].ownership.num_houses < max_houses) {
            printf("You must sell evenly.\n");
            pause();
            continue;
        }

        house_price = locations[to_sell].house_price;
        credit_player(p_p, house_price*0.5);
        p_p->liquid_assets -= house_price/2;
        p_p->taxable_assets -= house_price;
        locations[to_sell].ownership.num_houses--;
        /* TODO: update player's total assets */
    }

}

/* TODO: allow player to build hotels */
void buy_houses(Player_t * p_p) {
    int i, j;
    int to_buy_m, to_buy_p;
    int house_price;
    int valid_choices[NUM_LOC];
    int valid_choice;

    for(i=0; i<NUM_LOC; i++)
        valid_choices[i] = 0;

choose_monopoly:
    printf("You own the following monopolies:\n");
    for(i=0; i < NUM_PROPERTY_GROUPS; i++) {

        /* TODO: skip over railroad and utilities */
        if(0)
            continue;

        /* TODO: make this not fucking ugly */
        if(monopoly_owner(locations+property_groups[i][1]) == p_p) {
            printf("%d. [",i);
            for(j=0; j < property_groups[i][0]; j++) {
                printf("%s", locations[property_groups[i][j+1]].name);
                if(j != property_groups[i][0]-1)
                    printf(", ");
            }
            printf("]\n");

            valid_choices[i] = 1;
        }
    }

    printf("For which monopoly would you like to buy houses: ");
    fgets(buffer,MAXCHARS,stdin);
    to_buy_m = atoi(buffer);

    valid_choice = (to_buy_m >= 0) && (to_buy_m < NUM_PROPERTY_GROUPS) && valid_choices[to_buy_m];

    if(!valid_choice) {
        printf("\n\n\nINVALID INPUT\n\n\n");
        goto choose_monopoly;
    }

    to_buy_p = 0;

    for(i=0; i<NUM_LOC; i++)
        valid_choices[i] = 0;

    while(to_buy_p != NUM_LOC) {
        int min_houses = MAX_HOUSES;

        printf("\n\n\n\n\n\n\n\n\n\n");

        for(j=0; j < property_groups[to_buy_m][0]; j++) {
            int k;
            int l_i = property_groups[to_buy_m][j+1];
            int num_houses = locations[l_i].ownership.num_houses;

            if(num_houses < min_houses)
                min_houses = num_houses;

            printf("%d. [",l_i);

            for(k=0; k < MAX_HOUSES; k++) {
                if(k < num_houses)
                    printf("X");
                else
                    printf(" ");
            }

            valid_choices[l_i] = 1;

            printf("] %s\n",locations[l_i].name);
        }
        printf("%d. done\n",NUM_LOC);

        fgets(buffer,MAXCHARS,stdin);
        to_buy_p = atoi(buffer);

        if(to_buy_p == NUM_LOC)
            continue;

        valid_choice = (to_buy_p >= 0) && (to_buy_p < NUM_LOC) && valid_choices[to_buy_p];

        if(!valid_choice) {
            printf("\n\n\nINVALID CHOICE\n\n\n");
            continue;
        }

        if(locations[to_buy_p].ownership.num_houses == MAX_HOUSES) {
            printf("This property already has %d houses.\n",MAX_HOUSES);
            printf("Would you like to buy a hotel?\n");
            continue;
        }

        if(locations[to_buy_p].ownership.num_houses > min_houses) {
            printf("You must build evenly.\n");
            continue;
        }

        house_price = locations[to_buy_p].house_price;
        printf("%s\n",locations[to_buy_p].name);

        if(p_p->cash > house_price) {
            debit_player(p_p, house_price);
            p_p->liquid_assets += house_price/2;
            p_p->taxable_assets += house_price;
            locations[to_buy_p].ownership.num_houses++;
            /* TODO: add house to player's total assets calculation */
        }
        else {
            printf("YOU DO NO HAVE SUFFICIENT FUNDS TO PURCHASE A HOUSE FOR THAT PROPERTY\n");
            pause();
            continue;
        }

    }

}

void do_auction(Game_state_t * gs_p, int l_i) {

    int num_participants = gs_p->num_players;
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
            pause();
        }
    }

    highest_bidder_p = &(gs_p->players[highest_bidder]);
    printf("sold! to %s for %d\n", highest_bidder_p->name, current_bid);

    debit_player(highest_bidder_p, current_bid);
    make_player_owner(highest_bidder_p, l_i);
}

void make_player_owner(Player_t * p_p, int l_i) {
    Location_t * l_p = &(locations[l_i]);

    /* take away from old owner, if there is one */
    if(l_p->ownership.owned) {
        Player_t * old_owner = l_p->owner;
        old_owner->property[l_i] = 0;
        old_owner->liquid_assets -= l_p->mortgage_value;
        old_owner->taxable_assets -= l_p->price;
        /* TODO: create assertion for 0 houses*/
    }

    /* and give to the new owner */
    p_p->property[l_i] = 1;
    l_p->ownership.owned = 1;
    l_p->owner = p_p;
    p_p->liquid_assets += l_p->mortgage_value;
    p_p->taxable_assets += l_p->price;
    printf("%s, you now own %s\n", p_p->name, l_p->name);
    if(monopoly_owner(l_p) == p_p)
        printf("you now own a monopoly!\n");
}

void enter_jail(Player_t * p_p) {
    p_p->in_jail = 1;
    p_p->rolls_in_jail = 0;
/*    p_p->roll_double_count = 0; */
}

/*
   Does the following:
   * rolls dice
   * keeps track of number of times player has rolled doubles consecutively
   * places player in jail if rolled doubles three times consecutively
   * advances the player's token (if they don't enter jail due to doubles)
   * invokes do_location_action, which does the appropriate thing for the given location
 */
void do_roll(Game_state_t * gs_p) {

    int p_i = gs_p->turn.player_index;
    Player_t * p_p = &(gs_p->players[p_i]);
    Location_t * l_p;

    roll_dice(gs_p);

    if(p_p->in_jail) {
        if(gs_p->turn.doubles) {
            printf("You rolled doubles and are out of jail!\n");
            p_p->in_jail = 0;
            gs_p->turn.doubles = 0; /* don't want to allow player to use these doubles */
        }
        else 
            return;
    }
    else {
        if(gs_p->turn.doubles) {
            printf("YOU ROLLED DOUBLES\n");
            pause();
            gs_p->turn.roll_double_count++;
        }
        else {
            gs_p->turn.roll_double_count = 0;
        }

        /* if roll double count is 3, go to jail, end turn */
        if(gs_p->turn.roll_double_count == 3) {
            printf("YOU ROLLED DOUBLES THREE TIMES IN A ROW. GO TO JAIL!\n");
            pause();
            enter_jail(p_p);
            return;
        }
    }

    advance_token(p_p,gs_p->turn.die1,gs_p->turn.die2);

    l_p = &(locations[p_p->location_index]);
    gs_p->turn.on_unowned = (l_p->location_type == PROPERTY_T) && (!l_p->ownership.owned);

    do_location_action(gs_p);
}

/* TODO*/
void offer_trade(Game_state_t * gs_p, Player_t * p_p) {

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
                    printf("You do not have a get-out-of-jail free card.\n");
                break;
            case CASH_R:
                printf("cash amount to request: ");
                fgets(buffer,MAXCHARS,stdin);
                cash_request = atoi(buffer);
                if(trade_details.partner->cash <= cash_request)
                    trade_details.cash_request = cash_request;
                else
                    printf("Trading partner does not have that much cash.\n");
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

    printf("Executing trade...\n");

    /* exchange cash */
    if(trade_details.cash_offer > 0) {
        credit_player(trade_details.partner,             trade_details.cash_offer);
        debit_player( trade_details.player_making_offer, trade_details.cash_offer);
    }
    if(trade_details.cash_request > 0) {
        debit_player( trade_details.partner,             trade_details.cash_offer);
        credit_player(trade_details.player_making_offer, trade_details.cash_offer);
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

    while(prop_index != NUM_LOC) {

        for(i=0; i<NUM_LOC; i++) {
            if(p_p->property[i] && locations[i].ownership.num_houses == 0) {
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
            if(p_p->property[prop_index])
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

    printf("Game over! The winner is: %s\n",gs_p->winner->name);

}

void end_game(Game_state_t * gs_p) {
    /* TODO: free allocated resources*/
}
