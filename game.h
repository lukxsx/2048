#ifndef GAME_
#define GAME_

typedef struct {
	int** game_array;
	int x_size;
	int y_size;
	int score;
	int moves;
	int startnum;
	int best;
} game_state_t;

enum movement {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

#endif
