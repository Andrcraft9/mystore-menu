#include <stdio.h>
#include <ncurses.h>
#include <menu.h>
#include <locale.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

int crypt_add(int c, int count, char* crypt)
{
    return (c + crypt[count]) % 256;
}

int crypt_rm(int c, int count, char* crypt)
{
    return ((c - crypt[count]) < 0) ? (c - crypt[count] + 256) : (c - crypt[count]);
}

std::string read_store(char *crypt, std::string namef)
{
    //def_prog_mode();
    //endwin(); 

    //Read
    std::string note;
    int f_fd = open(namef.c_str(),O_RDONLY);
    int count = 0;
    char buf[4096];
    ssize_t len;
    if (f_fd < 0)
    {
        //fprintf(stderr,"Error open file for reading\n");
        return "Error open file for reading\n";
    }
    while ((len = read(f_fd,buf,sizeof(buf))) > 0)
    {
        for (int i = 0; i < len; ++i)
        {
            buf[i] = (char) crypt_rm(buf[i],count, crypt);
            count = (count + 1) % 5;
        }
        //write(1, buf, len);
        std::string str(buf, len);
        note.append(str);
    }
    close(f_fd);

    //printf("%s \n", note.c_str());
    //getch();
    //reset_prog_mode();

    /*
    std::string note;
    std::ifstream f;
    f.open(namef);
    while (f >> c)
    {
        c = (char) crypt_rm(c, count, crypt);
        std::cout << c;
        note.push_back(c);
        count = (count + 1) % 5;
    }
    f.close();
    */

    return note;
}

class Menu
{
public:
    Menu(int x, int y, int w, int h, const std::vector<std::string> c) 
        : x(x), y(y), width(w), height(h), content(c), win(newwin(h, w, y, x))
    {
        keypad(win, TRUE);
        scrollok(win, TRUE);
        content.push_back("Exit");
        current_state = 0;

        /*
        items = (ITEM **) calloc(content.size()+1, sizeof(ITEM *));
        for(int i = 0; i < content.size(); ++i)
                items[i] = new_item(content[i].c_str(), content[i].c_str());
        items[content.size()] = (ITEM *) NULL;
        men = new_menu((ITEM **) items);
        set_menu_win(men, win);
        //set_menu_sub(men, derwin(win, h, w, 5, 5));
        set_menu_format(men, 4, 1);
        set_menu_mark(men, " * ");
        menu_opts_off(men, O_SHOWDESC);
        //mvwaddch(win, 2, 0, ACS_LTEE);
        //mvwhline(win, 2, 1, ACS_HLINE, 38);
        //mvwaddch(win, 2, 39, ACS_RTEE);
        */
    }

    ~Menu() 
    {
        wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
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

        //unpost_menu(men);
        //free_menu(men);
        //for(int i = 0; i < content.size(); ++i)
        //        free_item(items[i]);

        wclear(win);
        wrefresh(win);
        delwin(win);
    }

    std::string get_state_string(int state)
    {
        return content[state-1];
    }

    int control()
    {
        int c, choice;

        current_state = 1;
        this->show();
        while(1)
        {
            c = wgetch(win);
            switch(c)
            {
                case KEY_UP:
                    //menu_driver(men, REQ_UP_ITEM);
                    if(current_state == 1)
                        current_state = content.size();
                    else
                        --current_state;
                    break;
                case KEY_DOWN:
                    //menu_driver(men, REQ_DOWN_ITEM);
                    if(current_state == content.size())
                        current_state = 1;
                    else 
                        ++current_state;
                    break;
                case 10:
                    if (current_state == content.size())
                        return 0;
                    else
                        return current_state;
                    break;
                default:
                    break;
            }

            this->show();
        }

        return 0;    
    }

    void show()
    {
        int locx, locy;
        locx = 2;
        locy = 2;

        for(int i = 0; i < content.size(); ++i)
        {
            if(current_state == i + 1)
            {
                wattron(win, A_BOLD); 
                wattron(win, COLOR_PAIR(1));
                mvwprintw(win, locy, locx, "%s", content[i].c_str());
                wattroff(win, A_BOLD);
                wattroff(win, COLOR_PAIR(1));
            }
            else
                mvwprintw(win, locy, locx, "%s", content[i].c_str());
            ++locy;
        }
        
        //post_menu(men);

        box(win, 0, 0);
        wrefresh(win);
    }

private:
    WINDOW *win;
    //ITEM **items;
    //MENU *men;
    
    int current_state;

    int x;
    int y;
    int width;
    int height;
    std::vector<std::string> content;

    // No copy
    Menu(const Menu& m);
    Menu& operator=(Menu const&);
};

class TextBox
{
public:
    TextBox(int x, int y, int w, int h, const std::string c) 
        : x(x), y(y), width(w), height(h), content(c), win(newwin(h, w, y, x))
    {
        keypad(win, TRUE);
        scrollok(win, TRUE);
        //clearok(win, TRUE);
        //idlok(win, TRUE);
        //immedok(win, TRUE);
        //wsetscrreg(win, 0, 100);
    }

    ~TextBox() 
    {
        wclear(win);
        wrefresh(win);
        delwin(win);
    }

    void show() const
    {
        int c;
        mvwprintw(win, 1, 1, "%s", content.c_str());
        wrefresh(win);

        while(1)
        {
            c = wgetch(win);
            switch(c)
            {
                case KEY_UP:
                    wscrl(win, -1);
                    break;
                case KEY_DOWN:
                    wscrl(win, 1); 
                    break;
                case 10:
                    return;
                    break;
                default:
                    return;
                    break;
            }
        }
    }

private:
    WINDOW *win;

    int x;
    int y;
    int width;
    int height;
    std::string content;

    // No copy
    TextBox(const TextBox& m);
    TextBox& operator=(TextBox const&);
};

int main()
{   
    setlocale(LC_CTYPE, "ru_RU.UTF8"); // +lncursesw = russian language

    initscr(); { // All objects need be deleted before endwin() call
    curs_set(0);
    if (has_colors() == FALSE)
    {
        endwin();
        printf("Your terminal does not support color\n");
        return 1;
    }
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    clear();
    noecho();
    cbreak();

    mvprintw(0, 0, "Welcome to mystore-menu, alpha version 0.0.1");
    refresh();

    Menu main_menu(0, 1, 30, 10, 
        {"MyStore, Create", "MyStore, Read", "TODO, Local List", "TODO, Global List"});
    int main_state;

    while(main_state = main_menu.control())
    {
        switch(main_state)
        {
            case 2:
            {
                char crypt[50];
                def_prog_mode(); /* Save the tty modes */
                endwin(); /* End curses mode temporarily */
                printf("Please enter password: \n");
                scanf("%s", crypt);
                reset_prog_mode(); /* Return to the previous tty mode*/

                std::string path = "/home/andr/mstor/";
                std::vector<std::string> vs;
                for (const auto & p : fs::directory_iterator(path))
                    vs.push_back(p.path().u8string());
                Menu r_menu(0, 12, 60, 30, vs);
                int r_state;

                while(r_state = r_menu.control())
                {
                    std::string namef = r_menu.get_state_string(r_state);
                    std::string note = read_store(crypt, namef);

                    TextBox noteBox(70, 1, 120, 80, note);
                    noteBox.show();
                }
                break;
            }
            default:
                break;
        }
        
        mvprintw(12, 0, "Your choice is %s \n", main_menu.get_state_string(main_state).c_str());
        refresh();
    }
    
    } endwin(); // All objects need be deleted before endwin() call
    return 0;
}

