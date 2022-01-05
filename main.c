#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>
#include "gamestate.h"
#include "game.h"
#include "textui.h"


int set_param(char* str, int* num) {
	char *end;
	errno = 0;
	long value = strtol(str, &end, 10);
	if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN))
			|| (errno != 0 && value == 0)) {
		return 0;
	}
	if (end == str) {
		return 0;
	}
	if (value <= 0) {
		return 0;
	}
	*num = (int) value;
	return 1;
}

int process_args(int argc, char** argv, game_state_t * game) {
	int opt;
	while ((opt = getopt(argc, argv, ":x:y:n:h")) != -1) {
		switch (opt) {
			case 'x':
				if (!set_param(optarg, &game->x_size)) {
					fprintf(stderr, "Incorrect argument %s\n", optarg);
					printusage(argv[0]);
					return 0;
				}
				break;
			case 'y':
				if (!set_param(optarg, &game->y_size)) {
					fprintf(stderr, "Incorrect argument %s\n", optarg);
					printusage(argv[0]);
					return 0;
				}
				break;
			case 'n':
				if (!set_param(optarg, &game->startnum)) {
					fprintf(stderr, "Incorrect argument %s\n", optarg);
					printusage(argv[0]);
					return 0;
				}
				break;
			case 'h':
				printusage(argv[0]);
				return 0;
			case '?':
				printf("Unknown option: %c\n", optopt);
				printusage(argv[0]);
				return 0;
				break;
			case ':':
				printf("Missing argument for %c\n", optopt);
				printusage(argv[0]);
				return 0;
				break;
		}
	}
	return 1;
}

int main(int argc, char** argv) {
	srand(time(NULL));

	// initialize game options
	game_state_t * game = malloc(sizeof(game_state_t));
	game->x_size = 4;
	game->y_size = 4;
	game->score = 0;
	game->moves = 0;
	game->startnum = 2;
	game->best = game->startnum;

	// process command line arguments
	if (!process_args(argc, argv, game)) {
		free(game);
		return EXIT_FAILURE;
	}


	// create main game array
	game->game_array = create_game_array(game);
	if (!game->game_array) {
		return -1;
	}


	// change terminal mode
	static struct termios old_terminal, new_terminal;
	tcgetattr(STDIN_FILENO, &old_terminal);
	new_terminal = old_terminal;
	new_terminal.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);

	// create two initial tiles
	create_random_tile(game);
	create_random_tile(game);
	print_array(game);

	// main game loop
	while (1) {
		if (is_full(game)) break;

		char key = getchar();
		switch (key) {
			case 'w':
				move(game, UP);
				break;
			case 'a':
				move(game, LEFT);
				break;
			case 's':
				move(game, DOWN);
				break;
			case 'd':
				move(game, RIGHT);
				break;
			default:
				break;
		}

	}

	printf("\nGAME OVER!\n");

	// free everything
	for (int j = 0; j < game->y_size; j++) {
		free(game->game_array[j]);
	}
	free(game->game_array);
	free(game);

	// return terminal back to the previous state
	tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
	return 0;
}
