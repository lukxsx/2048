/*
 This can be used for testing the game with automatic random input
*/

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
	int delay = 40000;
	srand(time(NULL));
	if (argc == 2) {
		delay = atoi(argv[1]);
	}
	while (1) {
		int n = rand() % 4;
		if (n == 0) printf("w");
		if (n == 1) printf("a");
		if (n == 2) printf("s");
		if (n == 3) printf("d");
		fflush(stdout);
		usleep(delay);
	}
}
