/* Conway's Game of life
 * Written in C with ncurses
 * Author: Pratyaksh Gautam */
#include <string.h>
#include "life.h"

int win_max[2] = {0, 0};
int delay = 200000;

int main() //{{{
{
	// Initialize ncurses
	initscr();
	cbreak();
	noecho();
    nodelay(stdscr, FALSE);			// Wait at getch() for input, don't return ERR
    keypad(stdscr, TRUE);
    curs_set(FALSE);

	// Decorate stdscr and draw border
	getmaxyx(stdscr, win_max[0], win_max[1]);
	box(stdscr, ACS_VLINE, ACS_HLINE);
	refresh();

	mainMenu(win_max);
	refresh();

	refresh();
	getch();
	endwin();
	return 0;
} //}}}

void mainMenu(int win_max[]) //{{{
{
	char *choices[] = {
		"Start",
		"Load",
		"Exit"
	};
	int num_choices = sizeof(choices) / sizeof(char *);

	char *banner[] = {
		" _     _  __      ",
		"| |   (_)/ _| ___ ",
		"| |   | | |_ / _ \\",
		"| |___| |  _|  __/",
		"|_____|_|_|  \\___|",
	};
	int banner_height = sizeof(banner) / sizeof(char *);

	int max_str_len = 0;
	for (int i = 0; i < num_choices; i++)	// the length of the longest string in choices
		max_str_len = (max_str_len > strlen(choices[i]))?
					  max_str_len: strlen(choices[i]);

	WINDOW * main_menu;
	int menu_height = num_choices + banner_height +  3 * MENU_PADDING ;
	// 2 * MENU_PADDING for top and bottom, + 1 * MENU_PADDING between banner and menu
	int menu_width = 40;
	// The next two variables calculate the left position to get centered
	// and left aligned text in the menu
	int menu_left_pad = (menu_width - max_str_len) / 2;
	int banner_left_pad = (menu_width - strlen(banner[0])) / 2;
	// create window and print the banner
	main_menu = newwin(menu_height, menu_width,
			(win_max[0] - menu_height) / 2, (win_max[1] - menu_width) / 2);
	box(main_menu, ACS_VLINE, ACS_HLINE);
	for (int i = 0; i < banner_height; i++) {
		mvwprintw(main_menu, i + MENU_PADDING, banner_left_pad, banner[i]);
	}

	// For printing the menu with the highlighted choices
	int choice = 0;
	int highlight = 1;
	while (choice == 0) {
		for (int i = 0; i < num_choices; i++) {
			// print the highlighted choice
			if ((i + 1) == highlight) {
				wattron(main_menu, A_REVERSE);
				mvwprintw(main_menu, i + banner_height + 2 * MENU_PADDING,
						menu_left_pad, choices[i]);
				wattroff(main_menu, A_REVERSE);
			}
			// print other choices
			else
				mvwprintw(main_menu, i + banner_height + 2 * MENU_PADDING,
						menu_left_pad, choices[i]);
		}
		wrefresh(main_menu);

		// Select an option from the choices
		int c = getch();
		switch (c) {
			case KEY_UP:
				if (highlight == 1)
					highlight = num_choices;		// Bounce back to the bottom
				else
					--highlight;
				break;
			case KEY_DOWN:
				if (highlight == num_choices)
					highlight = 1;				// Bounce back to the top
				else
					++highlight;
				break;
			case 10:					// Select the currently highlighted option
				choice = highlight;
				break;
			default:
				refresh();
				break;
		}
		wrefresh(main_menu);

		switch(choice) {
			case 1: // Start
				mvprintw(2, 2, "Starting!\n");
				wclear(main_menu);
				wrefresh(main_menu);
				delwin(main_menu);
				gameStart();
				break;
			case 2: // Load
				mvprintw(2, 2, "Loading!\n");
				wclear(main_menu);
				wrefresh(main_menu);
				delwin(main_menu);
				gameLoad();
				break;
			case 3:	// Exit
				return;
		}
	}
} //}}}

/* Allow the user to set up the game and then start it */
void gameStart() //{{{
{
	int game_height = win_max[0] - 2 * GAME_PADDING;
	int game_width = win_max[1] - 2 * GAME_PADDING;
	WINDOW *game_window = newwin(game_height, game_width,
			(win_max[0] - game_height) / 2, (win_max[1] - game_width) / 2);
	bool matrix[game_height][game_width];

	// Initialize the game board to all blank
	for (int i = 0; i < game_height; i++)
		for (int j = 0; j < game_width; j++)
			matrix[i][j] = false;

	if ('q' == gameSetup(game_window, &matrix[0][0], game_height, game_width))
			return;
	
	int inp;
	nodelay(game_window, TRUE);
	while( (inp = wgetch(game_window)) != 'q') {
		gameGeneration(game_window, &matrix[0][0], game_height, game_width);
	}
} //}}}

/* Load an existing game config, allow user to make any edits and then start it */
void gameLoad() //{{{
{
} //}}}
