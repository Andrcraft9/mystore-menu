#include "nstools.h"


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