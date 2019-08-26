#include "todomenu.h"

//    0: Show: show todo list
//    1: Add: add note to todo list
//    2: Finish: finish task, move to archive
//    3: Delete: delete task
//    4: Show archive: show archive
int TodoMenu::action(int id)
{
    switch(id)
    {
        case 0:
        {
            // Show todo list
            std::ifstream ftodo;
            ftodo.open(CONF_PATH + "todo_list");
            std::vector<std::string> vs;
            std::string line;
            while(std::getline(ftodo, line))
            {
                if (!line.empty())
                    vs.push_back(line);
            }
            ftodo.close();
            TodoNotes todolist(WidgetProperty(1 + 2*wp.menu1_x*(wp.screenRow), 1, 0.45*(1.0 - 2*wp.menu1_x)*(wp.screenRow), (wp.screenCol-3)), vs);
            todolist.control();
            break;
        }
        case 1:
        {
            // Add note to todo list
            def_prog_mode();
            endwin();
            printf("Please enter todo: \n");
            std::string todoname;
            std::getline(std::cin, todoname);
            
            std::ofstream ftodo;
            ftodo.open(CONF_PATH + "todo_list", std::ios_base::app);
            ftodo << todoname << std::endl;
            ftodo.close();
            reset_prog_mode();
            print_status_bar(wp.screenCol, wp.screenRow, COLR_HIGH, "Todo is created!");
            break;
        }
    }

    return id;
}