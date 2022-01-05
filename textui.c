#include <stdio.h>
#include "gamestate.h"

void print_top(int x_size) {
	printf("╔═══════");
	for (int i = 0; i < x_size-1; i++) {
		printf("╦═══════");
	}
	printf("╗\n");
}

void print_middle_walls(int x_size) {
	printf("║       ");
		for (int i = 0; i < x_size-1; i++) {
			printf("║       ");
		}
		printf("║\n");
}

void print_row_lines(int x_size) {
	printf("╠═══════");
	for (int i = 0; i < x_size-1; i++) {
		printf("╬═══════");
	}
	printf("╣\n");
}


void print_array(game_state_t * game) {
	printf("SCORE: %d   MOVES: %d   BEST: %d\n", game->score, game->moves, game->best);
	print_top(game->x_size);
	for (int j = 0; j < game->y_size; j++) {
		print_middle_walls(game->x_size);

		for (int i = 0; i < game->x_size; i++) {
			int num = game->game_array[j][i];
			if (num == 0) {
				printf("║       ");
			} else if (num >= 1000) {
				printf("║ %d  ", num);
			} else if (num >= 100 && num < 1000) {
				printf("║  %d  ", num);
			} else if (num >= 10 && num < 100) {
				printf("║   %d  ", num);
			} else {
				printf("║   %d   ", num);
			}

		}
		printf("║\n");

		print_middle_walls(game->x_size);
		if (j < game->y_size - 1) {
			print_row_lines(game->x_size);
		}
	}
	printf("╚═══════");
	for (int i = 0; i < game->x_size-1; i++) {
		printf("╩═══════");
	}
	printf("╝\n");
}

void printusage(char * program_name) {
	fprintf(stderr, "Simple command line 2048 clone. Can be played with WASD keys.\n");
	fprintf(stderr, "Usage: %s [options]\n", program_name);
	fprintf(stderr, "Available options:\n");
	fprintf(stderr, "    -x <value>    Set horizontal size (default 4, min 1)\n");
	fprintf(stderr, "    -y <value>    Set vertical size (default 4, min 1)\n");
	fprintf(stderr, "    -n <value>    Set start tile value (default 2, min 1)\n");
	fprintf(stderr, "    -h            Show this information\n");
}
