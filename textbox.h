#include <stdio.h>
#include <ncurses.h>
#include <menu.h>

#include <vector>
#include <string>
#include <iostream>

class TextBox
{
public:
    TextBox(int x, int y, int w, int h, int fore, int back, const std::string c) 
        : x(x), y(y), width(w), height(h), foreground(fore), background(back), content(c), win(newwin(h, w, y, x))
    {
        keypad(win, TRUE);
        scrollok(win, TRUE);
        //clearok(win, TRUE);
        //idlok(win, TRUE);
        //immedok(win, TRUE);
        //wsetscrreg(win, 0, 100);
    }

    ~TextBox() 
    {
        wclear(win);
        wrefresh(win);
        delwin(win);
    }

    void show() const
    {
        int c;
        mvwprintw(win, 1, 1, "%s", content.c_str());
        wrefresh(win);

        while(1)
        {
            c = wgetch(win);
            switch(c)
            {
                case KEY_UP:
                    wscrl(win, -1);
                    break;
                case KEY_DOWN:
                    wscrl(win, 1); 
                    break;
                case 10:
                    return;
                    break;
                default:
                    return;
                    break;
            }
        }
    }

private:
    WINDOW *win;

    int x;
    int y;
    int width;
    int height;
    int foreground; 
    int background;
    std::string content;

    // No copy
    TextBox(const TextBox& m);
    TextBox& operator=(TextBox const&);
};


class TextPadBox
{
public:
    TextPadBox(int x, int y, int w, int h, int fore, int back, const std::string c) 
        : x(x), y(y), width(w), height(h), foreground(fore), background(back), content(c)
    {
        
        int newlines = 0;
        for (int i = 0; i < content.length(); i++)
            if (content[i] == '\n') newlines++;
        PadHeight = (content.length()/w + newlines);
        //PadHeight = 4*newlines;

        win = newpad(PadHeight, w);
        keypad(win, TRUE);
    }

    ~TextPadBox() 
    {
        delwin(win);
        
        // How clear pad correctly? Dont know...
        win = newwin(height+5, width+5, y, x);
        wclear(win);
        wrefresh(win);
        delwin(win);
    }

    void show() const
    {
        int c, cols;
        cols = 0;
        //mvwprintw(win, 0, 0, "%s", content.c_str()); // WTF with mvprintw? try print large text
        waddstr(win, content.c_str());
        prefresh(win, cols, 0, y, x, y + height, x + width);
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

            prefresh(win, cols, 0, y, x, y + height, x + width);
            refresh();
        }
    }

private:
    WINDOW *win;

    int x;
    int y;
    int width;
    int height;
    int PadHeight;
    int foreground;
    int background;
    std::string content;

    // No copy
    TextPadBox(const TextPadBox& m);
    TextPadBox& operator=(TextPadBox const&);
};