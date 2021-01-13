#include "life.h"

/* Set up the boolean matrix for playing life */
/* returns 'p' to continue to play, 'q' to quit to main menu */
char gameSetup(WINDOW *game_window, bool *matrix, int game_height, int game_width) //{{{
{
	curs_set(1);	// Make cursor visible for now
	keypad(game_window, TRUE);
	wclear(game_window);
	int highlight[2];	// Set up a highight at roughly the center of the window
	highlight[0] = game_height / 2;
	highlight[1] = game_width / 2;
	wmove(game_window, highlight[0], highlight[1]);

	// Print initial lattice
	for (int i = 0; i < game_height; i++)
		for (int j = 0; j < game_width; j++)
			if ( *(matrix + i * game_width + j) )
				mvwaddch(game_window, i, j, ACS_BLOCK);
	wrefresh(game_window);

	int inp;
	while( (inp = wgetch(game_window)) != 'q' && inp != 'p') {	// q to quit, p to play
		switch (inp) {
			case KEY_UP:
				highlight[0] = MOD_DEC(highlight[0], game_height);
				break;
			case KEY_DOWN:
				highlight[0] = MOD_INC(highlight[0], game_height);
				break;
			case KEY_LEFT:
				highlight[1] = MOD_DEC(highlight[1], game_width);
				break;
			case KEY_RIGHT:
				highlight[1] = MOD_INC(highlight[1], game_width);
				break;
			case 10:	// Enter key
				// Invert the boolean for the cell
				*(matrix + highlight[0] * game_width + highlight[1]) = !(*(matrix + highlight[0] * game_width + highlight[1]));
				// And make the corresponding visual change
				if ( *(matrix + highlight[0] * game_width + highlight[1]) )
					mvwaddch(game_window, highlight[0], highlight[1], ACS_BLOCK);
				else
					mvwaddch(game_window, highlight[0], highlight[1], ' ');
				break;
		}

		wmove(game_window, highlight[0], highlight[1]);
		wrefresh(game_window);
	}
	curs_set(0);	// Make cursor invisible again
	return inp;
} //}}}

/* Takes a boolean matrix for where the cells in the matrix are alive
 * prints the whole thing in the given window */
void gameGeneration(WINDOW *game_window, bool *matrix, int game_height, int game_width) //{{{
{
	wclear(game_window);
	// set up a count for the number of live neighbors and init to zero
	int live_neighbors[game_height][game_width];
	for (int i = 0; i < game_height; i++)
		for (int j = 0; j < game_width; j++)
			live_neighbors[i][j] = 0;


	// Iterate over each element in the matrix
	for (int i = 0; i < game_height; i++) {
		for (int j = 0; j < game_width; j++) {

			// If the cell is alive...
			if ( *(matrix + i * game_width + j) ) {
				mvwaddch(game_window, i, j, ACS_BLOCK);		// ...print it on the right place in the grid, ... 

				live_neighbors[MOD_INC(i, game_height)][j]++;
				live_neighbors[MOD_DEC(i, game_height)][j]++;
				live_neighbors[i][MOD_INC(j, game_width)]++;
				live_neighbors[i][MOD_DEC(j, game_width)]++;
				live_neighbors[MOD_INC(i, game_height)][MOD_INC(j, game_width)]++;
				live_neighbors[MOD_INC(i, game_height)][MOD_DEC(j, game_width)]++;
				live_neighbors[MOD_DEC(i, game_height)][MOD_INC(j, game_width)]++;
				live_neighbors[MOD_DEC(i, game_height)][MOD_DEC(j, game_width)]++;
				// ... and increase increment live_neighbors for each of it's neighbors
			}
		}
	}

	// Now to see if a cell survives onto the next generation
	for (int i = 0; i < game_height; i++) {
		for (int j = 0; j < game_width; j++) {
			if ( *(matrix + i * game_width + j) ) {
				// if a live cell has less than 2 or more than 3 neighbors, 
				// it will not survive in the next generation
				if (live_neighbors[i][j] < 2 || live_neighbors[i][j] > 3)
					*(matrix + i * game_width + j)= false;
			}
			else {
				// if a dead cell has exactly three neighbors
				// it magically comes back to life
				if (live_neighbors[i][j] == 3)
					*(matrix + i * game_width + j)= true;
			}
		}
	}
	wrefresh(game_window);
	usleep(delay);
} //}}}
