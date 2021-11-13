#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	move_all_left(a, n);
	combine(a, n);
	move_all_left(a, n);
}

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

void move(int** game_array, enum movement dir) {	
	if (dir == DOWN) {
		for (int i = 0; i < x_size; i++) {
			int* temp = malloc(y_size*sizeof(int));
			for (int j = 0; j < y_size; j++) {
				temp[j] = game_array[j][i];
			}
			reverse_array(temp, y_size);
			move_single_array(temp, y_size);
			reverse_array(temp, y_size);
			for (int j = 0; j < y_size; j++) {
				game_array[j][i] = temp[j];
			}
			free(temp);
		}
	}
	if (dir == UP) {
		for (int i = 0; i < x_size; i++) {
			int* temp = malloc(y_size*sizeof(int));
			for (int j = 0; j < y_size; j++) {
				temp[j] = game_array[j][i];
			}
			move_single_array(temp, y_size);
			for (int j = 0; j < y_size; j++) {
				game_array[j][i] = temp[j];
			}
			free(temp);
		}
	}
	if (dir == LEFT) {
		for (int j = 0; j < y_size; j++) {
			move_single_array(game_array[j], x_size);
		}
	}
	if (dir == RIGHT) {
		for (int j = 0; j < y_size; j++) {
			int* temp = malloc(x_size*sizeof(int));
			temp = memcpy(temp, game_array[j], x_size*sizeof(int));
			reverse_array(temp, x_size);
			move_single_array(temp, x_size);
			reverse_array(temp, x_size);
			game_array[j] = memcpy(game_array[j], temp, x_size*sizeof(int));
			free(temp);
		}
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
    // main game loop
    while (1) {
		if (is_full(game_array)) break;
		create_random_tile(game_array, 2);
		print_array(game_array);
		
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

    for (int j = 0; j < y_size; j++) {
		free(game_array[j]);
	}
    free(game_array);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
    return 0;
}
