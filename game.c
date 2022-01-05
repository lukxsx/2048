#include <stdio.h>
#include <stdlib.h>
#include "gamestate.h"
#include "textui.h"

// creates two dimensional array filled with zeros
int** create_game_array(game_state_t * game) {
	int** game_array;
	game_array = malloc(game->y_size * sizeof(int *));
	if (!game_array) {
		exit(EXIT_FAILURE);
	}

	for (int j = 0; j < game->y_size; j++) {
		game_array[j] = malloc(game->x_size * sizeof(int));
		if (!game_array[j]) {
			for (int i = 0; i < j; i++) {
				free(game_array[i]);
			}
			free(game_array);
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < game->x_size; i++) {
			game_array[j][i] = 0;
		}
	}
	return game_array;
}

// checks if a tile in array is empty
int is_empty(int** game_array, int x, int y) {
	if (game_array[y][x] != 0) {
		return 0;
	}
	else return 1;
}

// returns true if the whole array is full (and game is over)
int is_full(game_state_t * game) {
	for (int j = 0; j < game->y_size; j++) {
		for (int i = 0; i < game->x_size; i++) {
			if (game->game_array[j][i] == 0) return 0;
		}
	}
	return 1;
}

// creates a new tile in random (empty) coordinates
void create_random_tile(game_state_t * game) {
	// make 2 with probability of 75%, 4 with probability of 25%
	int prob = (rand() & 1) | (rand() & 1);
	int value = prob ? game->startnum : game->startnum*2;
	int rx, ry;
	while (1) {
		rx = rand() % game->x_size;
		ry = rand() % game->y_size;
		if (is_empty(game->game_array, rx, ry)) {
			break;
		}
	}
	game->game_array[ry][rx] = value;
	game->best = value > game->best ? value : game->best;
}

// moves all tiles in array to left
void move_all_left(int* a, int n, int * modflag) {
	int last = 0;
	for (int i = 0; i < n; i++) {
		if (a[i] == 0) {
			continue;
		}
		if (a[i] != 0) {
			a[last] = a[i];
			if (i != last) {
				a[i] = 0;
				*modflag = 1;
			}

			last++;
		}
	}
}

// goes over an array and combines tiles with the same number
void combine(int* a, int n, game_state_t * game, int* modflag) {
	for (int i = 0; i < n; i++) {
		if (i < n-1) {
			if (a[i] == a[i+1] && a[i] != 0) {
				a[i] = a[i] * 2;
				game->score += a[i];
				*modflag = 1;
				a[i+1] = 0;
				game->best = a[i] > game->best ? a[i] : game->best;
			}
		}
	}
}

void move_single_array(int* a, int n, game_state_t * game, int * modflag) {
	move_all_left(a, n, modflag);
	combine(a, n, game, modflag);
	move_all_left(a, n, modflag);
}

// function for inverting a table
void reverse_array(int* array, int n) {
	int* temp = malloc(n*sizeof(int));

	for (int i = 0; i < n; i++) {
		temp[n - 1 - i] = array[i];
	}
	for (int i = 0; i < n; i++) {
		array[i] = temp[i];
	}
	free(temp);

}

// moves all tiles in the game to chosen direction
void move(game_state_t * game, enum movement dir) {
	int modified = 0;
	int *modflag = &modified;

	if (dir == DOWN) {
		for (int i = 0; i < game->x_size; i++) {
			int temp[game->y_size];
			for (int j = 0; j < game->y_size; j++) {
				temp[j] = game->game_array[j][i];
			}
			reverse_array(temp, game->y_size);
			move_single_array(temp, game->y_size, game, modflag);
			reverse_array(temp, game->y_size);
			for (int j = 0; j < game->y_size; j++) {
				game->game_array[j][i] = temp[j];
			}
		}
	}
	if (dir == UP) {
		for (int i = 0; i < game->x_size; i++) {
			int temp[game->y_size];
			for (int j = 0; j < game->y_size; j++) {
				temp[j] = game->game_array[j][i];
			}
			move_single_array(temp, game->y_size, game, modflag);
			for (int j = 0; j < game->y_size; j++) {
				game->game_array[j][i] = temp[j];
			}
		}
	}
	if (dir == LEFT) {
		for (int j = 0; j < game->y_size; j++) {
			move_single_array(game->game_array[j], game->x_size, game, modflag);
		}
	}
	if (dir == RIGHT) {
		for (int j = 0; j < game->y_size; j++) {
			reverse_array(game->game_array[j], game->x_size);
			move_single_array(game->game_array[j], game->x_size, game, modflag);
			reverse_array(game->game_array[j], game->x_size);
		}
	}
	if (*modflag) {
		create_random_tile(game);
		game->moves++;
		print_array(game);
	}
}

