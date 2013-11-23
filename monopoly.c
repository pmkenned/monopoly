#include "monopoly.h"

#define min(x,y) (x) < (y) ? (x) : (y)

/* TODO: property groups */
/* should this be a field in the location structure? */

/* TODO: rents, price for houses, price for hotels, mortgage price */
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
  /*                                       |--prices--|  |---rent----|  |-owernship-|       */
  /* name                    type          p    h  H  m   0 1 2 3 4 H    o  m  h  H   owner */
    {"GO",                   GO_T,         0,   0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Mediterranean Avenue", PROPERTY_T,   60,  0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Community Chest",      CHEST_T,      0,   0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Baltic Avenue",        PROPERTY_T,   60,  0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Income Tax",           INCOME_TAX_T, 0,   0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Reading Railroad",     PROPERTY_T,   200, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Oriental Avenue",      PROPERTY_T,   100, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Chance",               CHANCE_T,     0,   0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Vermont Avenue",       PROPERTY_T,   100, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Connecticut Avenue",   PROPERTY_T,   120, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Jail",                 JAIL_T,       0,   0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"St. Charles Place",    PROPERTY_T,   140, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Electric Company",     PROPERTY_T,   150, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"States Avenue",        PROPERTY_T,   140, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Virginia Avenue",      PROPERTY_T,   160, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Pennsylvania Railroad",PROPERTY_T,   200, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"St. James Place",      PROPERTY_T,   180, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Community Chest",      CHEST_T,      0,   0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Tennessee Avenue",     PROPERTY_T,   180, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"New York Avenue",      PROPERTY_T,   200, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Free Parking",         PARKING_T,    0,   0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Kentucky Avenue",      PROPERTY_T,   220, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Chance",               CHANCE_T,     0,   0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Indiana Avenue",       PROPERTY_T,   220, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Illinois Avenue",      PROPERTY_T,   240, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"B.&O. Railroad",       PROPERTY_T,   200, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Atlantic Avenue",      PROPERTY_T,   260, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Ventnor Avenue",       PROPERTY_T,   260, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Water Works",          PROPERTY_T,   150, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Marvin Gardens",       PROPERTY_T,   280, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"GO TO JAIL",           GO_TO_JAIL_T, 0,   0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Pacific Avenue",       PROPERTY_T,   300, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"North Carolina Avenue",PROPERTY_T,   300, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Community Chest",      CHEST_T,      0,   0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Pennsylvania Avenue",  PROPERTY_T,   320, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Short Line",           PROPERTY_T,   200, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Chance",               CHANCE_T,     0,   0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Park Place",           PROPERTY_T,   350, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Luxury Tax",           LUXURY_TAX_T, 0,   0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL},
    {"Boardwalk",            PROPERTY_T,   400, 0, 0, 0, {0,0,0,0,0,0}, {0, 0, 0, 0}, NULL}
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
    int num_players;
    char buffer[MAXCHARS];

    srand(time(NULL)); /* initialize random number generator */

    gs_p->game_over = 0;

    /* prompt for number of players */
    printf("how many players?\n");
    fgets(buffer,MAXCHARS,stdin);
    num_players = atoi(buffer); // TODO: handle edge cases
    gs_p->num_players = num_players;
    gs_p->players = malloc(sizeof(Player_t)*num_players); /* create players */

    /* init each player */
    for(i=0; i < num_players; i++) {
        Player_t * p_p = &(gs_p->players[i]);
        char ** name = &(p_p->name);
        int name_len;
        int j;

        printf("player %d:\n",i+1);

        /* allow each player to pick a name and piece type */
        printf("name: ");
        fgets(buffer,MAXCHARS,stdin);
        chomp(buffer);
        name_len = strlen(buffer);
        *name = malloc(sizeof(char)*(name_len+1));
        strncpy(*name,buffer,name_len);

//        printf("piece type: ");
//        fgets(buffer,MAXCHARS,stdin);
//        chomp(buffer);
        p_p->piece_type = 0; // TODO

        p_p->cash = 1500;
        p_p->location_index = GO_L;
        p_p->in_jail = 0;
        p_p->roll_double_count = 0;
        for(j=0; j<NUM_LOC; j++) {
            p_p->property[j].own = 0;
            p_p->property[j].num_houses = 0;
            p_p->property[j].hotel = 0;
        }
        p_p->has_get_out_of_jail_free = 0;
    }

    /* choose random starting player */
    int turn = rand() % num_players;
    printf("\n\nrandom starting player is %s\n",gs_p->players[turn].name);
    gs_p->turn = turn;
}

void advance_turn(Game_state_t * gs_p, int doubles) {
    int * turn = &(gs_p->turn);
    int num_players = gs_p->num_players;
    int j;

    Player_t * current_player_p = &(gs_p->players[*turn]);

   /* NOTE: by this point, turn has already ended.
      Thus, if player is in jail, the opportunity for doubles
      to get them out of jail has already passed */
    if(!doubles || current_player_p->in_jail)
        *turn = (*turn + 1) % num_players;

    Player_t * p_p = &(gs_p->players[*turn]);

    char * p_name = p_p->name;
    printf("================================\n");
    int name_len = strlen(p_name);
    char last_char = p_name[name_len-1];
    if(last_char == 's')
        printf("it is now %s' turn\n",p_name);
    else
        printf("it is now %s's turn\n",p_name);
    printf("================================\n");
    printf("You have: $%d cash on hand\n",p_p->cash);
    printf("You own: \n");
    // TODO: print houses, hotels, mortgaged
    for(j=0; j<NUM_LOC; j++) {
        if(p_p->property[j].own) {
            printf("%s\n",locations[j].name);
        }
    }
    printf("================================\n");
}

// TODO: disallow negative balances
void credit_or_debit_player(Game_state_t * gs_p, Player_t * p_p, int amount) {
    char * p_name = p_p->name;
    p_p->cash += amount;
    int coh = p_p->cash;
    if(amount > 0)
        printf("%s, your account has been credited $%d. ", p_name, amount);
    else
        printf("%s, your account has been debited $%d. ", p_name, -1*amount);
    printf("You now have $%d.\n",coh);
}

// TODO: write a function that can handle arbitrary moves
// including "go back 3 spaces" and "advance to boardwalk"
void advance_token(Game_state_t * gs_p, int die1, int die2) {

    int player_passed_go = 0;

    int turn = gs_p->turn;
    Player_t * p_p = &(gs_p->players[turn]);

    int * l_i = &(p_p->location_index);

    int sum = die1 + die2;

    int new_l_i = (*l_i+sum) % NUM_LOC;

    if(new_l_i < *l_i)
        player_passed_go = 1;

    (*l_i) = new_l_i;

    if(player_passed_go) {
        printf("You passed GO! Collect $200!\n");
        credit_or_debit_player(gs_p,p_p,200);
    }
}

void roll_dice(int * die1, int * die2) {
    *die1 = (rand() % 6)+1;
    *die2 = (rand() % 6)+1;
}

// TODO
int calculate_assets(Player_t * p_p) {
    return 0;
}

int calculate_rent(Game_state_t * gs_p, Location_t * l_p) {
    int owned      = l_p->ownership.owned;
    int mortgaged  = l_p->ownership.mortgaged;
    int num_houses = l_p->ownership.num_houses;
    int hotel      = l_p->ownership.hotel;

    int rent_index;

    if(!owned || mortgaged)
        return 0;

    if(!hotel)
        rent_index = num_houses;
    else
        rent_index = HOTEL;

    int rent = l_p->rent[rent_index];
}

void charge_rent(Game_state_t * gs_p, Player_t * p_p, Location_t * l_p) {

    int rent = calculate_rent(gs_p, l_p);
    printf("rent is: $%d\n",rent);

    // TODO: see if player has enough money
    //       otherwise, allow them to mortgage or trade
    if(1) {
        credit_or_debit_player(gs_p, p_p, rent);
    }

}

void land_on_property_action(Game_state_t * gs_p, Player_t * p_p, int l_i) {
    Location_t * l_p = &(locations[l_i]);

    int owned = l_p->ownership.owned;
    /* if unowned, see if player has enough money to buy it */
    if(!owned) {
        int choice;
        int price = l_p->price;
        printf("property is unowned; the price is $%d would you like to buy it?\n", price);
        choice = getchar(); // TODO: use fgets; user could type too many chars
        if(choice == 'y') {
            p_p->property[l_i].own = 1;
            credit_or_debit_player(gs_p, p_p, -1*price);
            l_p->ownership.owned = 1;
            l_p->owner = p_p;
        }
        else {
            // TODO: auction
        }
    }
    /* if owned, charge appropriate rent */
    else {
        Player_t * owner = l_p->owner;
        int owned_by_another = (owner == p_p) ? 0 : 1;
        if(owned_by_another) { // TODO: owned by another player
            printf("property is owned by another player\n");
            charge_rent(gs_p, p_p, l_p);
        }
        else
            printf("you own this property\n");
    }

}

void do_location_action(Game_state_t * gs_p) {

    int turn = gs_p->turn;
    Player_t * p_p = &(gs_p->players[turn]);
    int l_i = p_p->location_index;

    Location_t * l_p = &(locations[l_i]);
    char * l_name = l_p->name;
    int l_type = l_p->location_type;

    printf("you landed on %s\n",l_name);

    int assets, cash, ten_percent, least_amount;

    switch(l_type) {
        case GO_T:
            /* NOTE: advance_token gives the money; don't do it again here */
            break;
        case PROPERTY_T:
            land_on_property_action(gs_p,p_p,l_i);
            break;
        case CHEST_T:
            /* TODO: draw community chest card */
            break;
        case CHANCE_T:
            /* TODO: draw chance card */
            break;
        case INCOME_TAX_T:
            printf("You may pay either 10%% or $200\n");
            // TODO: 10% of total assets, not just cash
            assets = calculate_assets(p_p);
            cash = p_p->cash; // temporary
            ten_percent = cash/10;
            least_amount = min(200,ten_percent);
            credit_or_debit_player(gs_p, p_p, -1*least_amount);
            break;
        case LUXURY_TAX_T:
            credit_or_debit_player(gs_p, p_p, -75);
            break;
        case JAIL_T:
            /* just visiting */
            break;
        case GO_TO_JAIL_T:
            p_p->in_jail = 1;
            p_p->rolls_in_jail = 0;
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
void game_iter(Game_state_t * gs_p) {

    int turn = gs_p->turn;
    Player_t * p_p = &(gs_p->players[turn]);

    int in_jail = p_p->in_jail;

    if(in_jail) {
        char buffer[MAXCHARS];

        printf("player is in jail\n");
        printf("You may:\n");
        printf("1. choose to pay $50\n");
        printf("2. roll for doubles\n");
        int player_has_free_card = 0;
        if(player_has_free_card) {
            printf("3. use your get out of jail free card\n");
        }
        fgets(buffer, MAXCHARS, stdin);
        int choice = atoi(buffer);
        //TODO
        switch(choice) {
            case 1:
                credit_or_debit_player(gs_p, p_p, -50);
                // TODO: allow player to roll now
                p_p->in_jail = 0;
                p_p->roll_double_count = 0;
                p_p->rolls_in_jail = 0;
                break;
            case 2:
                break;
            case 3:
                p_p->in_jail = 0;
                p_p->has_get_out_of_jail_free = 0;
                break;
        }

        goto end_turn;
    }

    /* roll dice */
    int die1, die2;
    roll_dice(&die1, &die2);

    int doubles = (die1 == die2);

    printf("die 1: %d\ndie 2: %d\n",die1,die2);

    if(doubles) {
        printf("you rolled doubles\n");
        p_p->roll_double_count++;
    }
    else {
        p_p->roll_double_count = 0;
    }

    /* if roll double count is 3, go to jail, end turn */
    if(p_p->roll_double_count == 3) {
        printf("you rolled doubles three times in a row. you go to jail.\n");
        p_p->in_jail = 1;
        p_p->rolls_in_jail = 0;
        goto end_turn;
    }

    /* advance token; handles rolling doubles */
    advance_token(gs_p,die1,die2);

    do_location_action(gs_p);

end_turn:
    printf("press enter to advance turn\n");
    getchar();
    printf("\n\n\n\n\n\n");
    advance_turn(gs_p, doubles);
}

void game_loop(Game_state_t * gs_p) {

    while(!gs_p->game_over) {
        game_iter(gs_p);
    }

}

void end_game(Game_state_t * gs_p) {
    // TODO: free allocated resources
}
