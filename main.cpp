#include "menu.h"
#include "textbox.h"
#include "mystore.h"

#include <stdio.h>
#include <ncurses.h>
#include <menu.h>
#include <locale.h>

#include <ctime>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <cstdlib>

namespace fs = std::filesystem;


std::string get_current_date()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
    std::string str(buffer);

    return str;
}

int main()
{   
    setlocale(LC_CTYPE, "ru_RU.UTF8"); // +lncursesw = russian language

    // Config file
    std::ifstream fconf;
    fconf.open("config");
    
    float menu1_x, menu1_y;
    fconf >> menu1_x;
    fconf >> menu1_y;
    /*std::string line;
    while(std::getline(fconf, line))
    {
        if (line[0] != "#" )
        {
            std::istringstream iss(line);
            float num; 
            //while the iss is a number 
            while ((iss >> num))
            {
                //look at the number
            }
        }
    }*/
    fconf.close();

    initscr(); { // All objects need be deleted before endwin() call
    curs_set(0);
    if (has_colors() == FALSE)
    {
        endwin();
        printf("Your terminal does not support color\n");
        return 1;
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_RED);
    clear();
    noecho();
    cbreak();

    int screenRow, screenCol;
    getmaxyx(stdscr, screenCol, screenRow); 

    mvprintw(0, 0, "Welcome to mystore-menu, alpha version 0.0.4");
    refresh();

    Menu main_menu(0, 1, menu1_x*screenRow, menu1_y*(screenCol-3), 
        {"MyStore, Create", "MyStore, Read", "TODO, Local List", "TODO, Global List"});
    int main_state;

    while(main_state = main_menu.control())
    {
        switch(main_state)
        {
            case 1:
            {
                char crypt[50];
                def_prog_mode(); /* Save the tty modes */
                endwin(); /* End curses mode temporarily */
                std::system("nano note");

                printf("Note is written! \n");
                printf("Please enter password: \n");
                scanf("%s", crypt);

                std::string namef = STOR_PATH + get_current_date();
                write_store(crypt, "./note", namef);
                std::system("rm note");

                reset_prog_mode(); /* Return to the previous tty mode*/
                
                attron(COLOR_PAIR(3));
                mvprintw(screenCol-1, 0, "Note is MyStored!");
                refresh();
                attroff(COLOR_PAIR(3));

                break;
            }
            case 2:
            {
                char crypt[50];
                def_prog_mode();
                endwin();
                printf("Please enter password: \n");
                scanf("%s", crypt);
                reset_prog_mode();

                std::string path = STOR_PATH;
                std::vector<std::string> vs;
                for (const auto & p : fs::directory_iterator(path))
                    vs.push_back(p.path().u8string());
                Menu r_menu(0,  2 + menu1_y*(screenCol-2),  2*menu1_x*(screenRow),  (1.0 - menu1_y)*(screenCol-3), vs);
                int r_state;

                while(r_state = r_menu.control())
                {
                    std::string namef = r_menu.get_current_item_name(r_state);
                    std::string note = read_store(crypt, namef);

                    //TextPadBox noteBox(50, 4, 130, 40, note);
                    TextPadBox noteBox(1 + 2*menu1_x*(screenRow), 1, (1.0 - 2*menu1_x)*(screenRow), screenCol-3, note);
                    noteBox.show();
                }
                break;
            }
            default:
                break;
        }
        
        attron(COLOR_PAIR(3));
        mvprintw(screenCol-1, 0, "Your choice is %s \n", main_menu.get_current_item_name(main_state).c_str());
        refresh();
        attroff(COLOR_PAIR(3));
    }
    
    } endwin(); // All objects need be deleted before endwin() call
    return 0;
}