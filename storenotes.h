
#ifndef _STORE_NOTES_
#define _STORE_NOTES_

#include <vector>
#include <string>
#include <iostream>

#include "nstools.h"
#include "mystore.h"
#include "notes.h"
#include "textbox.h"

class StoreNotes: public Notes
{
public:
    StoreNotes(WidgetProperty wp,  std::vector<std::string> c, char cr[]);

private:
    char crypt[CRYPT_PSWD_MAX_LENGTH];
    // Action on menu click
    virtual int action(int id) override;
};

#endif
