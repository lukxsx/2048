#ifndef GAME_
#define GAME_

int** create_game_array(game_state_t * game);
int is_empty(int** game_array, int x, int y);
int is_full(game_state_t * game);
void create_random_tile(game_state_t * game);
void move_all_left(int* a, int n, int * modflag);
void combine(int* a, int n, game_state_t * game, int* modflag);
void move_single_array(int* a, int n, game_state_t * game, int * modflag);
void reverse_array(int* array, int n);
void move(game_state_t * game, enum movement dir);

#endif
