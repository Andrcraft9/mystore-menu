#include "mstor/todonotes.h"

TodoNotes::TodoNotes(WidgetProperty wp,  std::vector<std::string> c) : Notes(wp, c) 
{
    
}

int TodoNotes::action(int id)
{
    print_status_bar(wp.screenCol, wp.screenRow, COLR_HIGH, get_current_item_name(id));
    return id;
}