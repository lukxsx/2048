#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

typedef struct {
  int x_size;
  int y_size;
  int score;
  int moves;
} game_state_t;

enum movement {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

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


void print_array(int** game_array, game_state_t * game) {
	printf("SCORE: %d    MOVES: %d\n", game->score, game->moves);
	print_top(game->x_size);
	for (int j = 0; j < game->y_size; j++) {
		print_middle_walls(game->x_size);
		
		for (int i = 0; i < game->x_size; i++) {
			int num = game_array[j][i];
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

void printusage() {
	fprintf(stderr, "Usage: 2048 [options]\n");
	fprintf(stderr, "Available options:\n");
	fprintf(stderr, "    --x <value>      Set horizontal size (default 4)\n");
	fprintf(stderr, "    --y <value>      Set vertical size (default 4)\n");
	fprintf(stderr, "    --help         Show this information\n");

}

// checks if a tile in array is empty
int is_empty(int** game_array, int x, int y) {
	if (game_array[y][x] != 0) {
		return 0;
	}
	else return 1;
}

// returns true if the whole array is full (and game is over)
int is_full(int** game_array, game_state_t * game) {
	for (int j = 0; j < game->y_size; j++) {
		for (int i = 0; i < game->x_size; i++) {
			if (game_array[j][i] == 0) return 0;
		}
	}
	return 1;
}

// creates a new tile in random (empty) coordinates 
void create_random_tile(int** game_array, game_state_t * game) {
	int rx;
	int ry;
	while (1) {
		rx = rand() % game->x_size;
		ry = rand() % game->y_size;
		if (is_empty(game_array, rx, ry)) {
			break;
		}
	}
	game_array[ry][rx] = 2;
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
void move(int** game_array, game_state_t * game, enum movement dir) {
	int modified = 0;
	int *modflag = &modified;
	if (dir == DOWN) {
		for (int i = 0; i < game->x_size; i++) {
			int* temp = malloc(game->y_size*sizeof(int));
			for (int j = 0; j < game->y_size; j++) {
				temp[j] = game_array[j][i];
			}
			reverse_array(temp, game->y_size);
			move_single_array(temp, game->y_size, game, modflag);
			reverse_array(temp, game->y_size);
			for (int j = 0; j < game->y_size; j++) {
				game_array[j][i] = temp[j];
			}
			free(temp);
		}
	}
	if (dir == UP) {
		for (int i = 0; i < game->x_size; i++) {
			int* temp = malloc(game->y_size*sizeof(int));
			for (int j = 0; j < game->y_size; j++) {
				temp[j] = game_array[j][i];
			}
			move_single_array(temp, game->y_size, game, modflag);
			for (int j = 0; j < game->y_size; j++) {
				game_array[j][i] = temp[j];
			}
			free(temp);
		}
	}
	if (dir == LEFT) {
		for (int j = 0; j < game->y_size; j++) {
			move_single_array(game_array[j], game->x_size, game, modflag);
		}
	}
	if (dir == RIGHT) {
		for (int j = 0; j < game->y_size; j++) {
			int* temp = malloc(game->x_size*sizeof(int));
			temp = memcpy(temp, game_array[j], game->x_size*sizeof(int));
			reverse_array(temp, game->x_size);
			move_single_array(temp, game->x_size, game, modflag);
			reverse_array(temp, game->x_size);
			game_array[j] = memcpy(game_array[j], temp, game->x_size*sizeof(int));
			free(temp);
		}
	}
	if (*modflag) {
		create_random_tile(game_array, game);
		game->moves++;
		print_array(game_array, game);
	}
}


// converts strings to integers with error checking
int str2int(char* str) {
	char *end;
	errno = 0;
	long value = strtol(str, &end, 10);
	if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN))
            || (errno != 0 && value == 0)) {
		printusage();
        exit(EXIT_FAILURE);
    }
    if (end == str) {
		printusage();
        exit(EXIT_FAILURE);
    }
    return (int) value;
}

int main(int argc, char** argv) {
		
	// change terminal mode
    static struct termios old_terminal, new_terminal;
    tcgetattr(STDIN_FILENO, &old_terminal);
    new_terminal = old_terminal;
    new_terminal.c_lflag &= ~(ICANON | ECHO);          
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);
    srand(time(NULL));
    
    game_state_t * game = malloc(sizeof(game_state_t));
    game->x_size = 4;
    game->y_size = 4;
    game->score = 0;
    game->moves = 0;
    
    // process args
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			
			// get x size
			if (!strcmp(argv[i], "--x")) {
				if (argc > i+1) {
					game->x_size = str2int(argv[i+1]);
				}
			}
			
			// get y size
			if (!strcmp(argv[i], "--y")) {
				if (argc > i+1) {
					game->y_size = str2int(argv[i+1]);
				}
			}
			
			// help
			if (!strcmp(argv[i], "--help")) {
				printusage();
				exit(EXIT_SUCCESS);
			}
		}
		
	}
        
    int** game_array = create_game_array(game);
    if (!game_array) {
		return -1;
	}

    create_random_tile(game_array, game);
    create_random_tile(game_array, game);
    print_array(game_array, game);
    
    // main game loop
    while (1) {
		if (is_full(game_array, game)) break;
		
		char key = getchar();
		switch (key) {
			case 'w':
				move(game_array, game, UP);
				break;
			case 'a':
				move(game_array, game, LEFT);
				break;
			case 's':
				move(game_array, game, DOWN);
				break;
			case 'd':
				move(game_array, game, RIGHT);
				break;
			default:
				break;
		}
		
	}
	
	printf("\nGAME OVER!\n");

	for (int j = 0; j < game->y_size; j++) {
		free(game_array[j]);
	}
    free(game_array);
	free(game);
    // return terminal back to the previous state
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
    return 0;
}
