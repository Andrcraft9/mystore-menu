#include <stdio.h>
#include <ncurses.h>
#include <menu.h>

#include <vector>
#include <string>
#include <iostream>

#include "nstools.h"
#include "notes.h"

#ifndef _TODO_NOTES_
#define _TODO_NOTES_

class TodoNotes: public Notes
{
public:
    TodoNotes(WidgetProperty wp,  std::vector<std::string> c);

private:
    // Action on menu click
    virtual int action(int id) override;
};

#endif
