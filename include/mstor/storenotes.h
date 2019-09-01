
#ifndef _STORE_NOTES_
#define _STORE_NOTES_

#include <vector>
#include <string>
#include <iostream>

#include "mstor/nstools.h"
#include "mstor/mystore.h"
#include "mstor/notes.h"
#include "mstor/textbox.h"

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
