#include "storenotes.h"

StoreNotes::StoreNotes(WidgetProperty wp,  std::vector<std::string> c, char cr[]) :  Notes(wp, c)
{
    for(int i = 0; i < CRYPT_PSWD_MAX_LENGTH; ++i)
        crypt[i] = cr[i];
}

int StoreNotes::action(int id)
{
    //print_status_bar(wp.screenCol, wp.screenRow, COLR_HIGH, get_current_item_name(id));
    std::string namef = get_current_item_name(id);
    std::string note = read_store(crypt, namef);

    TextPadBox noteBox(WidgetProperty(WidgetProperty::TOP_BOTTOM_MIDDLE_RIGHT), note);
    noteBox.show();

    return id;
}