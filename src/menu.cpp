#include "mstor/menu.h"

Menu::Menu(WidgetProperty wp, std::vector<std::string> c) 
    : wp(wp), win(newwin(wp.h, wp.w, wp.y, wp.x)), content(c)
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
    set_menu_sub(men, derwin(win, wp.h-2, wp.w-2, 1, 1));
    set_menu_format(men, int(wp.h*0.8), 1);

    set_menu_mark(men, " * ");
    set_menu_back(men, COLOR_PAIR(wp.background));
    set_menu_fore(men, COLOR_PAIR(wp.foreground));
    set_menu_pad(men, COLOR_PAIR(wp.pad_color));
    menu_opts_off(men, O_SHOWDESC);

    wbkgd(win, COLOR_PAIR(wp.background));

    //mvprintw(1, 0, "Count %i", items_len);
    //refresh();
    //set_menu_sub(men, derwin(win, h-2, w-1, 0, 0));
    //mvwaddch(win, 2, 0, ACS_LTEE);
    // mvwhline(win, 2, 1, ACS_HLINE, 38);
    //mvwaddch(win, 2, 39, ACS_RTEE);
    
    wattron(win, COLOR_PAIR(wp.pad_color));
    box(win, 0, 0);
    wrefresh(win);
    wattroff(win, COLOR_PAIR(wp.pad_color));
    
    refresh();
    post_menu(men);
}

Menu::~Menu() 
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

std::string Menu::get_current_item_name(int id)
{
    if (id > items_len)
        return "UNKNOWN ID";
    ITEM *i;
    i = items[id];
    std::string iname(item_name(i));
    return iname;
}

int Menu::control()
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
                    return -1;
                else
                    action(icur);
                break;
            case 'q':
            case 'Q':
                return -1;
                break;
            default:
                break;
        }
        this->show();
    }

    return -1;    
}

void Menu::show()
{
    wrefresh(win);
}

int Menu::action(int id)
{
    // Do nothing
    return id;
}