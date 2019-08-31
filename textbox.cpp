#include "textbox.h"

TextPadBox::TextPadBox(WidgetProperty wp, const std::string c) : wp(wp), content(c)
{
    int newlines = 0;
    for (int i = 0; i < content.length(); i++)
        if (content[i] == '\n') newlines++;
    PadHeight = (content.length()/wp.w + newlines);
    //PadHeight = 4*newlines;

    win = newpad(PadHeight, wp.w);
    keypad(win, TRUE);
}

TextPadBox::~TextPadBox() 
{
    delwin(win);
    
    // How clear pad correctly? Dont know...
    win = newwin(wp.h + 5, wp.w + 5, wp.y, wp.x);
    wclear(win);
    wrefresh(win);
    delwin(win);
}

void TextPadBox::show()
{
    int c, cols;
    cols = 0;
    //mvwprintw(win, 0, 0, "%s", content.c_str()); // WTF with mvprintw? try print large text
    waddstr(win, content.c_str());
    prefresh(win, cols, 0, wp.y, wp.x, wp.y + wp.h, wp.x + wp.w);
    refresh();
    while(1)
    {
        c = wgetch(win);
        switch(c)
        {
            case KEY_UP:
            {
                if (cols <= 0) continue;
                cols--;
                break;
            }
            case KEY_DOWN:
            {
                //if (cols+height+1 >= PadHeight) continue;
                cols++;
                break;
            }
            case 10:
                return;
                break;
            default:
                return;
                break;
        }

        prefresh(win, cols, 0, wp.y, wp.x, wp.y + wp.h, wp.x + wp.w);
        refresh();
    }
}