/**
 * @file ui.cpp
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief https://tldp.org/HOWTO/NCURSES-Programming-HOWTO
 * @version See Git tags for version information.
 * @date 2022.03.16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <ncurses.h>
#include "meb_print.h"

WINDOW *InitWin(int col, int row, int cols, int rows)
{	WINDOW *local_win;

	local_win = newwin(rows, cols, row, col);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void DestroyWin(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}

typedef struct
{
    int data1;
    double data2;
    double data2;
} ui_data_t;

// Will replace 'int main()' when it becomes a thread.
// void *InterfaceThread(void *vp)
// {
//     ui_data_t *data = (ui_data_t *) vp;
int main()
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    refresh();

    WINDOW *win[3] = {0};

    win[0] = InitWin(0, 1, 80, 5);
    {
        mvwprintw(win[0], 0, 2, " OUTPUT ");
        wrefresh(win[0]);
    }

    win[1] = InitWin(0, 7, 60, 7);
    {
        mvwprintw(win[1], 0, 2, " TEST TITLE ");
        wrefresh(win[1]);
    }

    win[2] = InitWin(61, 7, 19, 7);
    {
        mvwprintw(win[2], 0, 2, " INPUT ");
        wrefresh(win[2]);
    }
 
    char ch = getch();

    for (int i = 0; i < (sizeof(win) / sizeof(WINDOW *)); i++)
    {
        DestroyWin(win[i]);
    }


/*     initscr();			// Start curses mode 		  
    raw(); // Disables line buffering.
    keypad(stdscr, true); // Enables function keys.
    noecho(); // Doesn't echo input during getch().

	printw("Type characters.\n");	// Print Hello World		  
    char ch = getch(); // Scans for a character. Using raw(), doesn't require pressing enter.

    if (ch == KEY_F(1))
    {
        printw("F1 key pressed!");
    }
    else
    {
        printw("The pressed key is ");
        attron(A_BOLD);
        printw("%c", ch);
        attroff(A_BOLD);
    }

    printw("\n\nPRESS ANY KEY TO EXIT...\n");
	refresh();			// Print it on to the real screen 
	getch();			// Wait for user input  */
	endwin();			//End curses mode		  

    return 0;
}