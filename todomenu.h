#include <stdio.h>
#include <ncurses.h>
#include <menu.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "nstools.h"
#include "menu.h"
#include "todonotes.h"

#ifndef _TODO_MENU_
#define _TODO_MENU_

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
    TodoMenu(WidgetProperty wp) : Menu(wp, {"Show", "Add", "Finish", "Delete", "Show archive"}) {}

private:
    virtual int action(int id) override;
};

#endif