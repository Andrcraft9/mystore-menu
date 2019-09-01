
#ifndef _TODO_MENU_
#define _TODO_MENU_

#include <ncurses.h>
#include <menu.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "mstor/nstools.h"
#include "mstor/menu.h"
#include "mstor/todonotes.h"

/* 
Contains:
    Show: show todo list
    Add: add note to todo list
    Finish: finish task, move to archive
    Delete: delete task
    Show archive: show archive
*/

class TodoMenu: public Menu
{
public:
    TodoMenu(WidgetProperty wp);

private:
    virtual int action(int id) override;
    std::vector<std::string> get_todolist(std::string fname);
};

#endif