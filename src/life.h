#ifndef LIFE_H
#define LIFE_H

#include <stdbool.h>
#include <unistd.h>
#include <curses.h>

#define MOD_DEC(a, n) ((a - 1) + n ) % n
#define MOD_INC(a, n) (a + 1) % n
#define MENU_PADDING 2
#define GAME_PADDING 2

char gameSetup(WINDOW *, bool *, int , int);
void gameGeneration(WINDOW *, bool *, int , int);
void mainMenu(int []);
void gameStart();
void gameLoad();

extern int win_max[2];
extern int delay;
#endif
