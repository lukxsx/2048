#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

int x_size = 4;
int y_size = 4;

enum movement {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

void print_array(int** game_array) {
	printf("╔═══");
	for (int i = 0; i < x_size-1; i++) {
		printf("╦═══");
	}
	printf("╗\n");
	for (int j = 0; j < y_size; j++) {
		printf("║");
		for (int i = 0; i < x_size; i++) {
			if (game_array[j][i] == 0) {
				if (i != 0) {
					printf("║   ");
				} else {
					printf("   ");
				}
			} else {
				if (i != 0) {
					printf("║ %d ", game_array[j][i]);
				} else {
				printf(" %d ", game_array[j][i]);
				}
			}
		}
		printf("║\n");
		if (j != y_size-1) {
			printf("╠═══");
			for (int i = 0; i < x_size-1; i++) {
				printf("╬═══");
			}
			printf("╣\n");
		}
	}
	printf("╚═══");
	for (int i = 0; i < x_size-1; i++) {
		printf("╩═══");
	}
	printf("╝\n");
}

int is_empty(int** game_array, int x, int y) {
	if (game_array[y][x] != 0) {
		return 0;
	}
	else return 1;
}

int is_full(int** game_array) {
	for (int j = 0; j < y_size; j++) {
		for (int i = 0; i < x_size; i++) {
			if (game_array[j][i] == 0) return 0;
		}
	}
	return 1;
}

void create_random_tile(int** game_array, int value) {
	int rx;
	int ry;
	while (1) {
		rx = rand() % 4;
		ry = rand() % 4;
		if (is_empty(game_array, rx, ry)) {
			break;
		}
	}
	game_array[ry][rx] = value;
}

void move_all_left(int* a, int n) {
	int last = 0;
	for (int i = 0; i < n; i++) {
		if (a[i] == 0) {
			continue;
		}
		if (a[i] != 0) {
			a[last] = a[i];
			if (i != last) {
				a[i] = 0;
			}
			last++;
		}
	}
}

void combine(int* a, int n) {
	for (int i = 0; i < n; i++) {
		if (i < n) {
			if (a[i] == a[i+1]) {
				a[i] = a[i] * 2;
				a[i+1] = 0;
			}
		}
	}
}

void move_single_array(int* a, int n) {
	move_left(a, n);
	combine(a, n);
	move_left(a, n);
}

void move(int** game_array, enum movement dir) {
	int* arr;
	if (dir == DOWN || dir == UP) {
		int* arr = malloc(y_size * sizeof(int));
	} else {
		int* arr = malloc(x_size * sizeof(int));
	}
	if (!arr) exit(EXIT_FAILURE);
	
	
	if (dir == DOWN) {
		
	}
	if (dir == UP) {
		printf("up\n");
	}
	if (dir == LEFT) {
		printf("left\n");
	}
	if (dir == RIGHT) {
		printf("right\n");
	}
}

int** create_game_array() {
	int** game_array;
	game_array = malloc(y_size * sizeof(int *));
    if (!game_array) {
		exit(EXIT_FAILURE);
	}
	
	for (int j = 0; j < y_size; j++) {
		game_array[j] = malloc(x_size * sizeof(int));
		if (!game_array[j]) {
			for (int i = 0; i < j; i++) {
				free(game_array[i]);
			}
			free(game_array);
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < x_size; i++) {
			game_array[j][i] = 0;
		}
	}
	return game_array;
}

int main(void) {
    static struct termios old_terminal, new_terminal;
    tcgetattr(STDIN_FILENO, &old_terminal);
    new_terminal = old_terminal;
    new_terminal.c_lflag &= ~(ICANON | ECHO);          
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);
    srand(time(NULL));
    
    int** game_array = create_game_array();
    if (!game_array) {
		return -1;
	}

    create_random_tile(game_array, 2);
    create_random_tile(game_array, 2);
    
    // main game loop
    while (1) {
		if (is_full(game_array)) break;
		char key = getchar();
		switch (key) {
			case 'w':
				move(game_array, UP);
				break;
			case 'a':
				move(game_array, LEFT);
				break;
			case 's':
				move(game_array, DOWN);
				break;
			case 'd':
				move(game_array, RIGHT);
				break;
			default:
				break;
		}
		
	}

    print_array(game_array);
    for (int j = 0; j < y_size; j++) {
		free(game_array[j]);
	}
    free(game_array);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
    return 0;
}
