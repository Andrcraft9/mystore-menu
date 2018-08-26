#include "menu.h"
#include "textbox.h"
#include "mystore.h"
#include "todobox.h"

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

enum
{
    COLR_RED = 1,
    COLR_GREEN = 2, 
    COLR_YELLOW = 3,
    COLR_BLUE = 4, 
    COLR_MAGENTA = 5,
    COLR_CYAN = 6, 
    COLR_DEFAULT = 7,
    COLR_MENU = 8,
    COLR_REVERT_MENU = 9,
    COLR_HIGH = 10
};

static const std::string CONF_PATH("./"); 
//static const std::string CONF_PATH("/home/andr/bin/mystore-menu-conf/"); 

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

std::string get_full_current_date()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    //strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
    strftime(buffer, sizeof(buffer), "%d %B %Y, %A", timeinfo);
    std::string str(buffer);

    return str;
}

void print_status_bar(int screenCol, int screenRow, int color, std::string msg)
{
    attron(COLOR_PAIR(color));
    mvhline(screenCol-1, 0, ' ', screenRow);
    mvprintw(screenCol-1, 0, "%s", msg.c_str());
    refresh();
    attroff(COLOR_PAIR(color));
}

int main()
{   
    setlocale(LC_CTYPE, "ru_RU.UTF8"); // +lncursesw = russian language

    // Config file
    std::ifstream fconf;
    fconf.open(CONF_PATH + "config");
    
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

    // Start ncurses mode
    initscr(); { // All objects need be deleted before endwin() call
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

    int screenRow, screenCol;
    getmaxyx(stdscr, screenCol, screenRow); 

    std::string hellos;
    hellos = get_full_current_date();
    attron(COLOR_PAIR(COLR_BLUE) | A_BOLD);
    mvprintw(0, (screenRow - hellos.size()) / 2, hellos.c_str());
    attroff(COLOR_PAIR(COLR_BLUE) | A_BOLD);
    print_status_bar(screenCol, screenRow, COLR_HIGH, "Welcome to mystore-menu (version 0.1.0)");

    Menu main_menu(0, 1, menu1_x*screenRow, menu1_y*(screenCol-3), 
            COLR_MENU, COLR_DEFAULT, COLR_REVERT_MENU,
            {"MyStore, Create", "MyStore, Read", "MyStore, Archive", "TODO"});
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
                char answer;
                printf("Do you want save it? (y/n) \n");
                std::cin >> answer;
                if (answer == 'y')
                {
                    printf("Please enter password: \n");
                    scanf("%s", crypt);
                    std::string namef = STOR_PATH + get_current_date();
                    write_store(crypt, "./note", namef);
                }
                std::system("rm note");

                reset_prog_mode(); /* Return to the previous tty mode*/
                
                if (answer == 'y')
                    print_status_bar(screenCol, screenRow, COLR_HIGH, "Note is MyStored!");
                else
                    print_status_bar(screenCol, screenRow, COLR_HIGH, "Note is deleted!");

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
                Menu r_menu(0,  2 + menu1_y*(screenCol-2),  2*menu1_x*(screenRow),  (1.0 - menu1_y)*(screenCol-3), 
                            COLR_MENU, COLR_DEFAULT, COLR_REVERT_MENU,
                            vs);
                int r_state;

                while(r_state = r_menu.control())
                {
                    std::string namef = r_menu.get_current_item_name(r_state);
                    std::string note = read_store(crypt, namef);

                    //TextPadBox noteBox(50, 4, 130, 40, note);
                    TextPadBox noteBox(1 + 2*menu1_x*(screenRow), 1, (1.0 - 2*menu1_x)*(screenRow), screenCol-3, 
                                       COLR_DEFAULT, COLR_DEFAULT, 
                                       note);
                    noteBox.show();
                }
                break;
            }
            case 3:
            {
                char crypt[50];
                def_prog_mode(); /* Save the tty modes */
                endwin(); /* End curses mode temporarily */

                std::string ifilename, ofilename;
                printf("Print full path of input file: \n");
                std::cin >> ifilename;
                printf("Print name of output file: \n");
                std::cin >> ofilename;

                char answer;
                printf("Do you want archive %s to %s? (y/n) \n", ifilename.c_str(), ofilename.c_str());
                std::cin >> answer;
                if (answer == 'y')
                {
                    printf("Please enter password: \n");
                    scanf("%s", crypt);
                    if (write_store(crypt, ifilename, STOR_PATH + ofilename) == 0)
                        std::system((std::string("rm ") + ifilename).c_str());
                    else
                    {
                        printf("Error in write_store! \n");
                        getch();
                    }
                }

                reset_prog_mode(); /* Return to the previous tty mode*/
                
                if (answer == 'y')
                    print_status_bar(screenCol, screenRow, COLR_HIGH, "Note is archived!");
                else
                    print_status_bar(screenCol, screenRow, COLR_HIGH, "Note is ignored");
        
                break;
            }
            case 4:
            {
                Menu main_menu2(1 + menu1_x*screenRow, 1, menu1_x*screenRow, menu1_y*(screenCol-3), 
                    COLR_MENU, COLR_DEFAULT, COLR_REVERT_MENU,
                    {"Show", "Add", "Delete", "Show archive"});
                int main2_state;
                main2_state = main_menu2.control();
                
                if (main2_state) 
                {
                    // Add todo list?
                    if (main2_state == 2) 
                    {
                        def_prog_mode();
                        endwin();
                        printf("Please enter todo: \n");
                        std::string todoname;
                        std::getline(std::cin, todoname);
                        
                        std::ofstream ftodo;
                        ftodo.open(CONF_PATH + "todo_list", std::ios_base::app);
                        ftodo << todoname << std::endl;
                        ftodo.close();
                        reset_prog_mode();

                        print_status_bar(screenCol, screenRow, COLR_HIGH, "Todo is created!");
                    }

                    // Show todo list
                    std::ifstream ftodo;
                    ftodo.open(CONF_PATH + "todo_list");
                    std::vector<std::string> vs;
                    std::string line;
                    while(std::getline(ftodo, line))
                    {
                        if (!line.empty())
                            vs.push_back(line);
                    }
                    ftodo.close();
                    TodoBox todo_loc_menu(1 + 2*menu1_x*(screenRow), 1, 0.45*(1.0 - 2*menu1_x)*(screenRow), (screenCol-3), 
                                       COLR_MENU, COLR_DEFAULT, COLR_REVERT_MENU,
                                       vs);
                    int todo_loc_state;

                    // Show archived todo list?
                    if (main2_state == 4) 
                    {
                        std::ifstream farchtodo;
                        farchtodo.open(CONF_PATH + "todo_archive");
                        std::vector<std::string> archvs;
                        std::string line;
                        while(std::getline(farchtodo, line))
                        {
                            if (!line.empty())
                                archvs.push_back(line);
                        }
                        farchtodo.close();
                        TodoBox todo_archive_loc_menu(1 + 2*menu1_x*(screenRow) + 1 + 0.45*(1.0 - 2*menu1_x)*(screenRow), 1, 0.45*(1.0 - 2*menu1_x)*(screenRow), (screenCol-3), 
                                        COLR_MENU, COLR_DEFAULT, COLR_REVERT_MENU,
                                        archvs);
                        int todo_archive_loc_state;
                        while(todo_archive_loc_state = todo_archive_loc_menu.control())
                            print_status_bar(screenCol, screenRow, COLR_HIGH, todo_archive_loc_menu.get_current_item_name(todo_archive_loc_state));
                        
                        break;
                    }

                    while(todo_loc_state = todo_loc_menu.control())
                    {
                        // Show item?
                        if (main2_state == 1) 
                        {
                            print_status_bar(screenCol, screenRow, COLR_HIGH, todo_loc_menu.get_current_item_name(todo_loc_state));
                        }

                        // Delete item?
                        if (main2_state == 3)
                        {
                            std::string dname = todo_loc_menu.get_current_item_name(todo_loc_state);
                            vs.erase(vs.begin() + todo_loc_state - 1);
                            std::ofstream ftodo;
                            ftodo.open(CONF_PATH + "todo_list");
                            for (auto& s : vs) 
                            {
                                ftodo << s << std::endl;
                            }
                            ftodo.close();

                            // Archive deleted todo
                            ftodo.open(CONF_PATH + "todo_archive", std::ios_base::app);
                            ftodo << dname << " | " << get_current_date() << std::endl;
                            ftodo.close();

                            print_status_bar(screenCol, screenRow, COLR_HIGH, "Todo is deleted (archived)!");

                            break;
                        }
                    }
                    
                }
                break;
            }
            case 5:
            {

            }
            default:
                break;
        }
    }
    
    } endwin(); // All objects need be deleted before endwin() call
    return 0;
}