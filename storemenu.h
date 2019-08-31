
#ifndef _STORE_MENU_
#define _STORE_MENU_

#include <algorithm>  
#include <ctime>
#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "mystore.h"
#include "nstools.h"
#include "menu.h"
#include "storenotes.h"
#include "todomenu.h"

/*
Contains:
    MyStore, Create: create note
    MyStore, Read: read notes
    MyStore, Archive: crypt created note
    TODO: Show todomenu
 */
class StoreMenu: public Menu
{
public:
    StoreMenu(WidgetProperty wp);
private:
    virtual int action(int id) override;
};

#endif