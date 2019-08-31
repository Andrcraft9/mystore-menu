
#ifndef _LOC_MENU_
#define _LOC_MENU_

#include <ncurses.h>
#include <menu.h>

#include <vector>
#include <string>
#include <iostream>

#include "nstools.h"

class Menu
{
public:
    Menu(WidgetProperty wp, std::vector<std::string> c);
    virtual ~Menu();

    std::string get_current_item_name(int id);
    // Main function. Show menu and take control of it.
    int control();

protected:
    WidgetProperty wp;
    WINDOW *win;
    MENU *men;
    ITEM **items;
    int items_len;
    std::vector<std::string> content;
    // Draw menu
    void show();

private:
    // No copy
    Menu(const Menu& m) = delete;
    Menu& operator=(Menu const&) = delete;
    // Action on menu click
    virtual int action(int id);
};

#endif