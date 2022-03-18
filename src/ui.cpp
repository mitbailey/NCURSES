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
#include <menu.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>
#include "ui.hpp"
#include "meb_print.h"

volatile sig_atomic_t done;

WINDOW *InitWin(int col, int row, int cols, int rows)
{
    WINDOW *local_win;

    local_win = newwin(rows, cols, row, col);
    box(local_win, 0, 0); // 0, 0 gives default characters for the vertical and horizontal lines.
    wrefresh(local_win);  // Show that box.

    return local_win;
}

void DestroyWin(WINDOW *local_win)
{
    /* box(local_win, ' ', ' '); : This won't produce the desired
     * result of erasing the window. It will leave it's four corners
     * and so an ugly remnant of window.
     */
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
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

void DestroyMenu(MENU *menu, int n_choices, ITEM **items)
{
    // Unpost and free all the memory taken up.
    unpost_menu(menu);
    free_menu(menu);
    for (int i = 0; i < n_choices; ++i)
    {
        free_item(items[i]);
    }
}

void WindowsInit(WINDOW *win[], float win_w[], float win_h[], int rows, int cols)
{
    int win0w = floor(win_w[0] * cols);
    int win0h = floor(win_h[0] * rows);
    int win1w = floor(win_w[1] * cols);
    int win1h = floor(win_h[1] * rows);
    int win2w = floor(win_w[2] * cols);
    int win2h = floor(win_h[2] * rows);

    if ((win1w + win2w + 1) > cols) { win2w = cols - win1w - 1; }

    win0h = (win0h < winmin_h[0]) ? winmin_h[0] : win0h;
    win1h = (win1h < winmin_h[1]) ? winmin_h[1] : win1h;
    win2h = (win2h < winmin_h[2]) ? winmin_h[2] : win2h;

    // int win1y = floor(1 + (win_h[0] * rows));
    // int win2y = floor(1 + (win_h[0] * rows));



    win[0] = InitWin(0, 1, win0w, win0h);
    {
        mvwprintw(win[0], 0, 2, " OUTPUT ");
        mvwprintw(win[0], 1, 2, "IN");
        mvwprintw(win[0], 1, floor(win0spcg * win0w), "OUT");
        mvwprintw(win[0], 1, 2 * floor(win0spcg * win0w), "STEP");
        mvwprintw(win[0], win0h - 1, win0w - 10, " %dx%d ", win0w, win0h);
        wrefresh(win[0]);
    }

    win[1] = InitWin(0, floor(1 + win0h), win1w, win1h);
    {
        mvwprintw(win[1], 0, 2, " WHEEL OF MISFORTUNE ");
        mvwprintw(win[1], win1h - 1, win1w - 10, " %dx%d ", win1w, win1h);
        wrefresh(win[1]);
    }

    win[2] = InitWin(floor(1 + win1w), floor(1 + win0h), win2w, win2h);
    {
        mvwprintw(win[2], 0, 2, " INPUT ");
        mvwprintw(win[2], 2, 2, "Selected:");
        mvwprintw(win[2], 3, 2, "Terminal Size: %d x %d", rows, cols);
        mvwprintw(win[2], win2h - 1, win2w - 10, " %dx%d ", win2w, win2h);
        wrefresh(win[2]);
    }
}

void WindowsDestroy(WINDOW *win[], int num_win)
{
    if (win == NULL || win == nullptr)
    {
        return;
    }

    for (int i = 0; i < num_win; i++)
    {
        DestroyWin(win[i]);
    }
}

void ncurses_init()
{
    initscr();
    cbreak();
    noecho(); // Doesn't echo input during getch().
    keypad(stdscr, TRUE);
}

void ncurses_cleanup()
{
    endwin();
    clear();
}

void sighandler(int sig)
{
    done = 1;
}

// To be replaced by an external main().
int main()
{
    signal(SIGINT, sighandler);

    // Example data package.
    ui_data_t data[1] = {0};

    // Handles the UI thread.
    pthread_t tid;
    pthread_create(&tid, NULL, InterfaceThread, data);

    for (int i = 0; (i < INT32_MAX) && (!done); i++)
    {
        // dbprintlf("%d %.0f %.0f", data->d1->peekData(), data->d2->peekData(), data->d3->peekData());
        data->d1->set(data->d1->peek() + 1);
        data->d2->set(data->d2->peek() + 1);
        data->d3->set(data->d3->peek() + 1);
    }

    pthread_join(tid, NULL);

    return 0;
}

void *InterfaceThread(void *vp)
{
    ui_data_t *data = (ui_data_t *)vp;

    ncurses_init();
    refresh();

    // Initialization and drawing of windows' static elements.
    int rows = 0, cols = 0;
    getmaxyx(stdscr, rows, cols);
    WindowsInit(win, win_w, win_h, rows, cols);
    
    // Menu1 setup.
    ITEM **menu1_items;
    int c;
    MENU *menu1;
    int menu1_n_choices, i;

    // generate_menu(menu1_items, menu1, menu1_n_choices);

    { // Generate the menu.
        menu1_n_choices = ARRAY_SIZE(menu1_choices);
        menu1_items = (ITEM **)calloc(menu1_n_choices, sizeof(ITEM *));
        for (i = 0; i < menu1_n_choices; ++i)
        {
            menu1_items[i] = new_item(menu1_choices[i], menu1_choices_desc[i]);
        }
        menu1 = new_menu((ITEM **)menu1_items);
        set_menu_win(menu1, win[1]);
        set_menu_sub(menu1, derwin(win[1], 6, 38, 2, 1));
        // set_menu_sub(my_menu, win[1]);
        set_menu_mark(menu1, " * ");
        post_menu(menu1);
        wrefresh(win[1]);
    }

    // Menu1 nav
    // Makes wgetch nonblocking so the menu isnt hogging all the cycles.
    nodelay(stdscr, true);
    while ((c = wgetch(stdscr)) != KEY_F(1) && (!done))
    {
        // Menu handling.
        switch (c)
        {
        case KEY_DOWN:
            menu_driver(menu1, REQ_DOWN_ITEM);
            wrefresh(win[1]);
            break;
        case KEY_UP:
            menu_driver(menu1, REQ_UP_ITEM);
            wrefresh(win[1]);
            break;
        case '\n':
            // TODO: Add functionality for other buttons.
            switch (item_index(current_item(menu1)))
            {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                goto program_end;
            }
            break;
        case KEY_RESIZE:
            // Clean-up.
            ncurses_cleanup();

            // Re-initialization.
            ncurses_init();
            refresh();

            getmaxyx(stdscr, rows, cols);
            WindowsInit(win, win_w, win_h, rows, cols);

            { // Generate the menu.
                menu1_n_choices = ARRAY_SIZE(menu1_choices);
                menu1_items = (ITEM **)calloc(menu1_n_choices, sizeof(ITEM *));
                for (i = 0; i < menu1_n_choices; ++i)
                {
                    menu1_items[i] = new_item(menu1_choices[i], menu1_choices_desc[i]);
                }
                menu1 = new_menu((ITEM **)menu1_items);
                set_menu_win(menu1, win[1]);
                set_menu_sub(menu1, derwin(win[1], 6, 38, 2, 1));
                // set_menu_sub(my_menu, win[1]);
                set_menu_mark(menu1, " * ");
                post_menu(menu1);
                wrefresh(win[1]);
            }
        }

        // Output Window Data Printouts
        static bool redraw = true;
        if (data->d1->rdy())
        {
            mvwprintw(win[0], 2, 2, "%d", data->d1->get());
            redraw = true;
        }

        if (data->d2->rdy())
        {
            mvwprintw(win[0], 2, floor(win0spcg * floor(win_w[0] * cols)), "%.2f", data->d2->get());
            redraw = true;
        }

        if (data->d3->rdy())
        {
            mvwprintw(win[0], 2, 2 * floor(win0spcg * floor(win_w[0] * cols)), "%.2f", data->d3->get());
            redraw = true;
        }

        if (redraw)
        {
            wrefresh(win[0]);
            redraw = false;
        }

        // Input Window
        static Data<int> m_idx(-1);
        m_idx.set(item_index(current_item(menu1)));
        if (m_idx.rdy())
        {
            mvwprintw(win[2], 2, 12, "%d", m_idx.get());
            wrefresh(win[2]);
        }
    }

program_end:
    DestroyMenu(menu1, menu1_n_choices, menu1_items);
    WindowsDestroy(win, ARRAY_SIZE(win));
    refresh();
    done = 1;
    ncurses_cleanup();

    return 0;
}