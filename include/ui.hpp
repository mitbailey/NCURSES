/**
 * @file ui.hpp
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief
 * @version See Git tags for version information.
 * @date 2022.03.17
 *
 * @copyright Copyright (c) 2022
 *
 */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

WINDOW *win[3] = {0};
float win_w[3] = {1.f, 0.6, 0.4};
float win_h[3] = {0.25, 0.5, 0.25};

template <class T>
class Data
{
public:
    Data() { this->data = 0; };
    Data(T data) { this->data = data; };

    void set(T data) 
    {
        if (this->data == data)
            return;
        this->data = data; 
        ready = true; 
    };

    bool rdy() { return ready; };

    T get()
    {
        if (ready)
        {
            ready = false;
            return data;
        }
        else
        {
            return 0;
        }
    };

    T peek() { return data; };

private:
    T data;
    bool ready;
};

typedef struct
{
    Data<int> d1[1];
    Data<double> d2[1];
    Data<double> d3[1];
} ui_data_t;

char *menu1_choices[] = {
    "1:",
    "2:",
    "3:",
    "4:",
    "Exit:",
    (char *)NULL,
};

char *menu1_choices_desc[] = {
    "Choice 1",
    "Choice 2",
    "Choice 3",
    "Choice 4",
    "Exits the Program",
    (char *)NULL,
};

/**
 * @brief Helper function for WindowsInit, initializes a single window.
 *
 * @param col
 * @param row
 * @param cols
 * @param rows
 * @return WINDOW*
 */
WINDOW *InitWin(int col, int row, int cols, int rows);

/**
 * @brief Helper function for WindowsDestroy, destroys a single window.
 *
 * @param local_win
 */
void DestroyWin(WINDOW *local_win);

/**
 * @brief Initializes all windows.
 *
 * @param win
 * @param win_w
 * @param win_h
 * @param rows
 * @param cols
 */
void WindowsInit(WINDOW *win[], float win_w[], float win_h[], int rows, int cols);

/**
 * @brief Destroys all windows.
 *
 * @param win
 * @param num_win
 */
void WindowsDestroy(WINDOW *win[], int num_win);

/**
 * @brief Performs NCURSES-specific initialization.
 *
 */
void ncurses_init();

/**
 * @brief Performs NCURSES-specific initialization.
 *
 */
void ncurses_cleanup();

// /**
//  * @brief Construct a new generate menu object
//  * 
//  * @param menu_items 
//  * @param menu 
//  * @param n_choices 
//  */
// void generate_menu(ITEM **menu_items, MENU *menu, int n_choices);

/**
 * @brief The main interface thread where all of the action happens.
 *
 * @param vp
 * @return void*
 */
void *InterfaceThread(void *vp);