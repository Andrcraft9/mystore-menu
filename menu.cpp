#include <stdio.h>
#include <ncurses.h>
#include <menu.h>
#include <locale.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#include <ctime>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <cstdlib>

namespace fs = std::filesystem;

static const std::string STOR_PATH("/home/andr/mstor/");

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

int write_store(char *crypt, std::string notef, std::string namef)
{
    int f_fd, newf_fd;
    int count;
    char buf[4096];
    ssize_t len;
    
    f_fd = open(notef.c_str(), O_RDONLY);
    if (f_fd < 0)
    {
        return 1;
    }

    newf_fd = open(namef.c_str(), O_WRONLY|O_CREAT|O_APPEND, 0666);
    if (newf_fd < 0) 
    {
        return 1;
    }

    //Read, write and crypt
    count = 0;
    while ((len=read(f_fd, buf, sizeof(buf))) > 0)
    {
        for(int i = 0; i < len; ++i)
        {
            buf[i] = (char) crypt_add(buf[i], count, crypt);
            count = (count + 1) % 5;
        }
        write(newf_fd,buf,len);
    }
    close(f_fd);
    close(newf_fd);

    return 0;
}

class Menu
{
public:
    Menu(int x, int y, int w, int h, std::vector<std::string> c) 
        : x(x), y(y), width(w), height(h), win(newwin(h, w, y, x)), content(c)
    {
        keypad(win, TRUE);
        //scrollok(win, TRUE);

        items_len = content.size() + 1;
        items = (ITEM **) calloc(items_len + 1, sizeof(ITEM *));
        for(int i = 0; i < items_len - 1; ++i)
            items[i] = new_item(content.at(i).c_str(), content.at(i).c_str());
        items[items_len - 1] = new_item("Exit", "Exit");
        items[items_len] = (ITEM *) NULL;
        men = new_menu((ITEM **) items);
        set_menu_win(men, win);
        set_menu_format(men, int(h*0.8), 1);
        set_menu_mark(men, " * ");
        set_menu_back(men, COLOR_PAIR(1));
        set_menu_fore(men, COLOR_PAIR(2));
        set_menu_pad(men, COLOR_PAIR(2));
        menu_opts_off(men, O_SHOWDESC);

        mvprintw(1, 0, "Count %i", items_len);
        refresh();
        //set_menu_sub(men, derwin(win, h-2, w-1, 0, 0));
        //mvwaddch(win, 2, 0, ACS_LTEE);
        //mvwhline(win, 2, 1, ACS_HLINE, 38);
        //mvwaddch(win, 2, 39, ACS_RTEE);
        box(win, 0, 0);
        post_menu(men);
    }

    ~Menu() 
    {
        //wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
        unpost_menu(men);
        free_menu(men);
        for(int i = 0; i < items_len; ++i)
            free_item(items[i]);

        wclear(win);
        wrefresh(win);
        delwin(win);
    }

    std::string get_current_item_name(int id)
    {
        if (id > items_len)
            return "UNKNOWN ID";
        ITEM *i;
        i = items[id-1];
        std::string iname(item_name(i));
        return iname;
    }

    int control()
    {
        int c, icur;
        ITEM *cur;

        this->show();
        while(1)
        {
            c = wgetch(win);
            switch(c)
            {
                case KEY_UP:
                    menu_driver(men, REQ_UP_ITEM);
                    break;
                case KEY_DOWN:
                    menu_driver(men, REQ_DOWN_ITEM);
                    break;
                case 10:
                    cur = current_item(men);
                    icur = item_index(cur);
                    if (icur == items_len-1)
                        return 0;
                    else
                        return icur+1; 
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
        wrefresh(win);
    }

private:
    WINDOW *win;
    MENU *men;
    ITEM **items;
    int items_len;

    std::vector<std::string> content;

    int x;
    int y;
    int width;
    int height;

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
        //scrollok(win, TRUE);
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
                    //wscrl(win, -1);
                    break;
                case KEY_DOWN:
                    //wscrl(win, 1); 
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

class TextPadBox
{
public:
    TextPadBox(int x, int y, int w, int h, const std::string c) 
        : x(x), y(y), width(w), height(h), content(c)
    {
        
        int newlines = 0;
        for (int i = 0; i < content.length(); i++)
            if (content[i] == '\n') newlines++;
        PadHeight = ((content.length() - newlines)/w + newlines + 1);

        win = newpad(PadHeight, w);
        keypad(win, TRUE);
    }

    ~TextPadBox() 
    {
        delwin(win);
        
        // How clear pad correctly? Dont know...
        win = newwin(height+5, width+5, y, x);
        wclear(win);
        wrefresh(win);
        delwin(win);
    }

    void show() const
    {
        int c, cols;
        cols = 0;
        mvwprintw(win, 0, 0, "%s", content.c_str());
        //waddstr(win, content.c_str());
        refresh();
        while(1)
        {
            c = wgetch(win);
            switch(c)
            {
                case KEY_UP:
                {
                    if (cols <= 0) continue;
                    cols--;
                    break;
                }
                case KEY_DOWN:
                {
                    if (cols+height+1 >= PadHeight) continue;
                    cols++;
                    break;
                }
                case 10:
                    return;
                    break;
                default:
                    return;
                    break;
            }

            prefresh(win, cols, 0, y, x, y + height, x + width);
            refresh();
        }
    }

private:
    WINDOW *win;

    int x;
    int y;
    int width;
    int height;
    int PadHeight;
    std::string content;

    // No copy
    TextPadBox(const TextPadBox& m);
    TextPadBox& operator=(TextPadBox const&);
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
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    clear();
    noecho();
    cbreak();

    mvprintw(0, 0, "Welcome to mystore-menu, alpha version 0.0.1");
    refresh();

    Menu main_menu(0, 3, 30, 7, 
        {"MyStore, Create", "MyStore, Read (Pad)", "MyStore, Read (Win)", "TODO, Local List", "TODO, Global List"});
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
                mvprintw(1, 0, "Note is MyStored!");
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
                Menu r_menu(0, 10, 50, 20, vs);
                int r_state;

                while(r_state = r_menu.control())
                {
                    std::string namef = r_menu.get_current_item_name(r_state);
                    std::string note = read_store(crypt, namef);

                    TextPadBox noteBox(70, 4, 100, 40, note);
                    noteBox.show();
                }
                break;
            }
            case 3:
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
                Menu r_menu(0, 10, 50, 20, vs);
                int r_state;

                while(r_state = r_menu.control())
                {
                    std::string namef = r_menu.get_current_item_name(r_state);
                    std::string note = read_store(crypt, namef);

                    TextBox noteBox(70, 4, 100, 40, note);
                    noteBox.show();
                }
                break;
            }
            default:
                break;
        }
        
        attron(COLOR_PAIR(3));
        mvprintw(1, 0, "Your choice is %s \n", main_menu.get_current_item_name(main_state).c_str());
        refresh();
        attroff(COLOR_PAIR(3));
    }
    
    } endwin(); // All objects need be deleted before endwin() call
    return 0;
}

