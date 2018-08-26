#include <stdio.h>
#include <ncurses.h>
#include <menu.h>

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class TodoBox
{
public:
    TodoBox(int x, int y, int w, int h, int foreground, int background, int pad_color, std::vector<std::string> c) 
        : x(x), y(y), width(w), height(h), win(newwin(h, w, y, x)), content(c)
    {
        keypad(win, TRUE);
       
        items_len = content.size() + 1;
        items = (ITEM **) calloc(items_len + 1, sizeof(ITEM *));
        for(int i = 0; i < items_len - 1; ++i)
        {
            //std::stringstream ss;
            //ss << i + 1;
            //std::string s = ss.str();
            //content[i] = s + ". " + content[i];
            items[i] =  new_item((content[i]).c_str(), (content[i]).c_str());
        }
        items[items_len - 1] = new_item("Exit", "Exit");
        items[items_len] = (ITEM *) NULL;
        men = new_menu((ITEM **) items);
        
        set_menu_win(men, win);
        set_menu_sub(men, derwin(win, h-2, w-2, 1, 1));
        set_menu_format(men, int(h*0.8), 1);

        set_menu_mark(men, " > ");
        set_menu_back(men, COLOR_PAIR(background));
        set_menu_fore(men, COLOR_PAIR(foreground));
        set_menu_pad(men, COLOR_PAIR(pad_color));
        menu_opts_off(men, O_SHOWDESC);

        wbkgd(win, COLOR_PAIR(background));
        
        wattron(win, COLOR_PAIR(pad_color));
        //box(win, '#', '#');
        wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(win);
        wattroff(win, COLOR_PAIR(pad_color));
        
        refresh();
        post_menu(men);
        wrefresh(win);
    }

    ~TodoBox() 
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

    void update_content()
    {
        //int set_menu_items(MENU *menu, ITEM **items);
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
    TodoBox(const TodoBox& m);
    TodoBox& operator=(TodoBox const&);
};
