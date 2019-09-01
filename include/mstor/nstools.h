
#ifndef _NCURSES_LOCAL_TOOLS_
#define _NCURSES_LOCAL_TOOLS_

#include <stdio.h>
#include <cassert>
#include <ncurses.h>
#include <menu.h>

#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <tuple>

#define CRYPT_PSWD_MAX_LENGTH 50

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

bool sortbydate(std::tuple<std::string, std::time_t> a, std::tuple<std::string, std::time_t> b);

std::string get_current_date();

std::string get_full_current_date();

void print_status_bar(int screenCol, int screenRow, int color, std::string msg);

class WidgetProperty
{
public:
    // Position of widget in screen
    enum Position
    {
        TOP_LEFT,
        TOP_MIDDLE,
        TOP_RIGHT,
        TOP_LEFT_MIDDLE,
        TOP_MIDDLE_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_MIDDLE,
        BOTTOM_RIGHT,
        BOTTOM_LEFT_MIDDLE,
        BOTTOM_MIDDLE_RIGHT,
        TOP_BOTTOM_MIDDLE_RIGHT
    };
    // Screen property
    static int screenRow, screenCol;
    // Size of each widget position (in percents of screen size)
    static const float menu1_x;
    static const float menu1_y;
    // Margin between windows (in pixels)
    static const int margin_x;
    static const int margin_y;

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
            // Default top positions
            case TOP_LEFT:
                x = 0; y = 1;  
                w = menu1_x*(screenCol) - margin_x; h = menu1_y*(screenRow - 2) - margin_y;
                break;
            case TOP_MIDDLE:
                x = menu1_x*(screenCol); y = 1;
                w = menu1_x*(screenCol) - margin_x; h = menu1_y*(screenRow - 2) - margin_y;
                break;
            case TOP_RIGHT:
                x = 2*menu1_x*(screenCol); y = 1;
                w = menu1_x*(screenCol) - margin_x; h = menu1_y*(screenRow - 2) - margin_y;
                break;
            // Extended top positions
            case TOP_LEFT_MIDDLE:
                x = 0; y = 1;  
                w = 2*menu1_x*(screenCol) - margin_x; h = menu1_y*(screenRow - 2) - margin_y;
                break;
            case TOP_MIDDLE_RIGHT:
                x = 2*menu1_x*(screenCol); y = 1;
                w = 2*menu1_x*(screenCol) - margin_x; h = menu1_y*(screenRow - 2) - margin_y;
                break;
            // Deafult bottom positions
            case BOTTOM_LEFT:
                x = 0; y = 1 + menu1_y*(screenRow - 2);  
                w = menu1_x*(screenCol) - margin_x; h = menu1_y*(screenRow - 2) - margin_y;
                break;
            case BOTTOM_MIDDLE:
                x = menu1_x*(screenCol); y = 1 + menu1_y*(screenRow - 2);  
                w = menu1_x*(screenCol) - margin_x; h = menu1_y*(screenRow - 2) - margin_y;
                break;    
            case BOTTOM_RIGHT:
                x = 2*menu1_x*(screenCol); y = 1 + menu1_y*(screenRow - 2);  
                w = menu1_x*(screenCol) - margin_x; h = menu1_y*(screenRow - 2) - margin_y;
                break;
            // Extended bottom positions
            case BOTTOM_LEFT_MIDDLE:
                x = 0; y = 1 + menu1_y*(screenRow - 2);  
                w = 2*menu1_x*(screenCol) - margin_x; h = menu1_y*(screenRow - 2) - margin_y;
                break;
            case BOTTOM_MIDDLE_RIGHT:
                x = menu1_x*(screenCol); y = 1 + menu1_y*(screenRow - 2);  
                w = 2*menu1_x*(screenCol) - margin_x; h = menu1_y*(screenRow - 2) - margin_y;
                break;
            // Top and Bottom
            case TOP_BOTTOM_MIDDLE_RIGHT:
                x = menu1_x*(screenCol); y = 1;
                w = 2*menu1_x*(screenCol) - margin_x; h = (screenRow - 2) - margin_y;
                break;
            default:
                //throw "Invalid possition";
                assert("Invalid possition");
        }
    }
};

#endif
