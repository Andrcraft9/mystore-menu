
#ifndef _LOC_NOTES_
#define _LOC_NOTES_

#include <ncurses.h>
#include <menu.h>

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "nstools.h"

class Notes
{
public:
    Notes(WidgetProperty wp, std::vector<std::string> c);
    virtual ~Notes();

    std::string get_current_item_name(int id);
    // Main function. Show menu and take control of it.
    int control();
    // Only one touch and return control
    int one_touch_control();

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
    Notes(const Notes& m) = delete;
    Notes& operator=(Notes const&) = delete;
    // Action on menu click
    virtual int action(int id);
};

#endif