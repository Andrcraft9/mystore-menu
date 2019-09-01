
#ifndef _TODO_NOTES_
#define _TODO_NOTES_

#include <vector>
#include <string>
#include <iostream>

#include "mstor/nstools.h"
#include "mstor/notes.h"

class TodoNotes: public Notes
{
public:
    TodoNotes(WidgetProperty wp,  std::vector<std::string> c);

private:
    // Action on menu click
    virtual int action(int id) override;
};

#endif
