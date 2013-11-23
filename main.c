#include "monopoly.h"

int main(int argc, char ** argv) {

    Game_state_t gs;

    init_game(&gs);

    game_loop(&gs);
    
    end_game(&gs);

    return 0;
}
