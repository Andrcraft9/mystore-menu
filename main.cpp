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
    attron(COLOR_PAIR(3));
    mvhline(screenCol-1, 0, ' ', screenRow);
    attroff(COLOR_PAIR(3));
    refresh();

    Menu main_menu(0, 1, menu1_x*screenRow, menu1_y*(screenCol-3), 
        {"MyStore, Create", "MyStore, Read", "MyStore, Archive", "TODO, Local List", "TODO, Global List"});
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
                
                attron(COLOR_PAIR(3));
                mvhline(screenCol-1, 0, ' ', screenRow);
                if (answer == 'y')
                    mvprintw(screenCol-1, 0, "Note is MyStored!");
                else
                    mvprintw(screenCol-1, 0, "Note is deleted!");
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
                
                attron(COLOR_PAIR(3));
                mvhline(screenCol-1, 0, ' ', screenRow);
                if (answer == 'y')
                    mvprintw(screenCol-1, 0, "Note is archived!");
                else
                    mvprintw(screenCol-1, 0, "Note is ignored");
                refresh();
                attroff(COLOR_PAIR(3));

                break;
            }
            case 4:
            {
                Menu main_menu2(1 + menu1_x*screenRow, 1, menu1_x*screenRow, menu1_y*(screenCol-3), 
                    {"Show", "Add", "Delete"});
                int main2_state;
                main2_state = main_menu2.control();
                
                if (main2_state) 
                {
                    if (main2_state == 2) // Add
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

                        attron(COLOR_PAIR(3));
                        mvhline(screenCol-1, 0, ' ', screenRow);
                        mvprintw(screenCol-1, 0, "Todo is created!");
                        refresh();
                        attroff(COLOR_PAIR(3));
                    }

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

                    Menu todo_loc_menu(1 + 2*menu1_x*(screenRow), 1, 0.3*(1.0 - 2*menu1_x)*(screenRow), (screenCol-3), vs);
                    int todo_loc_state;
                    while(todo_loc_state = todo_loc_menu.control())
                    {
                        if (main2_state == 1) // Show
                        {
                            attron(COLOR_PAIR(3));
                            mvhline(screenCol-1, 0, ' ', screenRow);
                            mvprintw(screenCol-1, 0, "todo: %s", todo_loc_menu.get_current_item_name(todo_loc_state).c_str());
                            refresh();
                            attroff(COLOR_PAIR(3));
                        }

                        if (main2_state == 3) // Delete
                        {
                            //std::string dname = todo_loc_menu.get_current_item_name(todo_loc_state);
                            vs.erase(vs.begin() + todo_loc_state - 1);

                            std::ofstream ftodo;
                            ftodo.open(CONF_PATH + "todo_list");
                            for (auto& s : vs) 
                            {
                                ftodo << s << std::endl;
                            }
                            ftodo.close();

                            attron(COLOR_PAIR(3));
                            mvhline(screenCol-1, 0, ' ', screenRow);
                            mvprintw(screenCol-1, 0, "Todo is deleted!");
                            refresh();
                            attroff(COLOR_PAIR(3));

                            break;
                        }
                    }

                    
                }
                break;
            }
            default:
                break;
        }
        
        //attron(COLOR_PAIR(3));
        //mvprintw(screenCol-1, 0, "Your choice is %s \n", main_menu.get_current_item_name(main_state).c_str());
        //refresh();
        //attroff(COLOR_PAIR(3));
    }
    
    } endwin(); // All objects need be deleted before endwin() call
    return 0;
}