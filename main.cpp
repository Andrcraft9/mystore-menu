#include "nstools.h"
#include "storemenu.h"
#include "menu.h"

#include <stdio.h>
#include <ncurses.h>
#include <menu.h>
#include <locale.h>

#include <algorithm>  
#include <ctime>
#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>

#include <cstdlib>

namespace fs = std::filesystem;

int WidgetProperty::screenRow;
int WidgetProperty::screenCol;
const float WidgetProperty::menu1_x = 0.33;
const float WidgetProperty::menu1_y = 0.5;
const int WidgetProperty::margin_x = 1;
const int WidgetProperty::margin_y = 1;

int main()
{   
    setlocale(LC_CTYPE, "ru_RU.UTF8"); // +lncursesw = russian language
    // Start ncurses mode
    initscr(); 
    { 
        // All objects need be deleted before endwin() call
        curs_set(0);
        if (has_colors() == FALSE)
        {
            endwin();
            printf("Your terminal does not support color\n");
            return 1;
        }
        start_color();
        int background;
        int foreground;
        if (use_default_colors() != ERR) 
        {
            background = -1;
            foreground = -1;
        }
        // Color assignment
        init_pair(COLR_RED, COLOR_RED, background);
        init_pair(COLR_GREEN, COLOR_GREEN, background);
        init_pair(COLR_YELLOW, COLOR_YELLOW, background);
        init_pair(COLR_BLUE, COLOR_BLUE, background);
        init_pair(COLR_MAGENTA, COLOR_MAGENTA, background);
        init_pair(COLR_CYAN, COLOR_CYAN, background);
        init_pair(COLR_DEFAULT, foreground, background);
        init_pair(COLR_MENU, COLOR_WHITE, COLOR_BLUE); // Selection in menu
        init_pair(COLR_REVERT_MENU, COLOR_BLUE, background); // Box around menu
        init_pair(COLR_HIGH, COLOR_WHITE, COLOR_BLUE); // Status bar
        // Init functions
        clear();
        noecho();
        cbreak();

        // Init screen and layout geometry 
        int screenRow, screenCol;
        getmaxyx(stdscr, screenRow, screenCol);
        WidgetProperty::screenRow = screenRow;
        WidgetProperty::screenCol = screenCol;

        // Welcome
        std::string hellos;
        hellos = get_full_current_date();
        attron(COLOR_PAIR(COLR_BLUE) | A_BOLD);
        mvprintw(0, (screenCol - hellos.size()) / 2, hellos.c_str());
        attroff(COLOR_PAIR(COLR_BLUE) | A_BOLD);
        print_status_bar(screenCol, screenRow, COLR_HIGH, "Welcome to mystore-menu (version 1.0.0)");

        // MyStore menu - main window, give a control to it
        StoreMenu store_menu = StoreMenu(WidgetProperty(WidgetProperty::TOP_LEFT));
        store_menu.control();

        //std::vector< std::string > vs = {"1", "2", "3"};
        //Menu m(WidgetProperty(WidgetProperty::TOP_LEFT), vs);
        //m.control();
    } 
    endwin(); // All objects need be deleted before endwin() call

    return 0;
}