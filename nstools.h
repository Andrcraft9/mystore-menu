#include <stdio.h>
#include <cassert>
#include <ncurses.h>
#include <menu.h>

#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <tuple>

#ifndef _NCURSES_LOCAL_TOOLS_
#define _NCURSES_LOCAL_TOOLS_

enum
{
    COLR_RED = 1,
    COLR_GREEN = 2, 
    COLR_YELLOW = 3,
    COLR_BLUE = 4, 
    COLR_MAGENTA = 5,
    COLR_CYAN = 6, 
    COLR_DEFAULT = 7,
    COLR_MENU = 8,
    COLR_REVERT_MENU = 9,
    COLR_HIGH = 10
};

//static const std::string CONF_PATH("./"); 
static const std::string CONF_PATH("/home/andr/bin/mystore-menu-conf/"); 

bool sortbydate(std::tuple<std::string, std::time_t> a, std::tuple<std::string, std::time_t> b) 
{ 
    return difftime(std::get<1>(a), std::get<1>(b)) > 0;
}

std::string get_current_date()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
    std::string str(buffer);

    return str;
}

std::string get_full_current_date()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    //strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
    strftime(buffer, sizeof(buffer), "%d %B %Y, %A", timeinfo);
    std::string str(buffer);

    return str;
}

void print_status_bar(int screenCol, int screenRow, int color, std::string msg)
{
    attron(COLOR_PAIR(color));
    mvhline(screenCol-1, 0, ' ', screenRow);
    mvprintw(screenCol-1, 0, "%s", msg.c_str());
    refresh();
    attroff(COLOR_PAIR(color));
}

class WidgetProperty
{
public:
    // Position of widget in screen
    enum Position
    {
        TOPLEFT,
        TOPMIDDLE,
        TOPRIGHT,
        BOTTOMLEFT,
        BOTTOMMIDDLE,
        BOTTOMRIGHT,
    };
    // Screen property
    static int screenRow, screenCol;
    // Size of each widget position in percents of screen size
    static const float menu1_x = 0.3;
    static const float menu1_y = 0.45;
    // Widget Position (x, y)
    int x, y; 
    // Widget size (width, height)
    int w, h; 
    // Colors
    int foreground, background; 
    int pad_color;

    WidgetProperty(int x, int y, int w, int h, int foreground = COLR_MENU, int background = COLR_DEFAULT, int pad_color = COLR_REVERT_MENU)
        : x(x), y(y), w(w), h(h), foreground(foreground), background(background), pad_color(pad_color) {}
    
    WidgetProperty(Position pos, int foreground = COLR_MENU, int background = COLR_DEFAULT, int pad_color = COLR_REVERT_MENU)
        : foreground(foreground), background(background), pad_color(pad_color) 
    {

        switch(pos)
        {
            case TOPLEFT:
                x = 0; y = 2 + menu1_y*(screenCol-2);  
                w = 2*menu1_x*(screenRow); h = (1.0 - menu1_y)*(screenCol-3);
                break;
            case TOPMIDDLE:

                break;
            case TOPRIGHT:

                break;
            case BOTTOMLEFT:

                break;
            case BOTTOMMIDDLE:

                break;    
            case BOTTOMRIGHT:

                break;
            default:
                //throw "Invalid possition";
                assert("Invalid possition");
        }
    }
};

#endif