#include "monopoly.h"

int main(int argc, char ** argv) {

    Game_state_t gs;
    int choice;

    while(1) {
        printf("Choose an option:\n");
        printf("1. New game\n");
        printf("2. Load game\n");
        fgets(buffer,MAXCHARS,stdin);
        choice = atoi(buffer);
        if(choice == 1) {
            new_game(&gs);
            break;
        }
        else if(choice == 2) {
            load_game(&gs);
            break;
        }
        else
            printf("Invalid input\n");
    }

    game_loop(&gs);
    
    end_game(&gs);

    return 0;
}
