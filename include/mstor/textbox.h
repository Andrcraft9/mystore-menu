
#ifndef _TEXT_BOX_
#define _TEXT_BOX_

#include <ncurses.h>
#include <menu.h>

#include <vector>
#include <string>
#include <iostream>

#include "mstor/nstools.h"

class TextPadBox
{
public:
    TextPadBox(WidgetProperty wp, const std::string c);
    ~TextPadBox();
    void show();

private:
    int PadHeight;
    WidgetProperty wp;
    WINDOW *win;
    std::string content;

    // No copy
    TextPadBox(const TextPadBox& m) = delete;
    TextPadBox& operator=(TextPadBox const&) = delete;
};

#endif