#include "todomenu.h"

TodoMenu::TodoMenu(WidgetProperty wp) : Menu(wp, {"Show", "Add", "Finish", "Delete", "Show archive"}) 
{

}

std::vector<std::string> TodoMenu::get_todolist(std::string fname)
{
    std::ifstream ftodo;
    ftodo.open(fname);
    std::vector<std::string> vs;
    std::string line;
    while(std::getline(ftodo, line))
    {
        if (!line.empty())
            vs.push_back(line);
    }
    ftodo.close();
    return vs;
}

//    0: Show: show todo list
//    1: Add: add note to todo list
//    2: Finish: finish task, move to archive
//    3: Delete: delete task
//    4: Show archive: show archive
int TodoMenu::action(int global_id)
{
    switch(global_id)
    {
        case 0:
        {
            // Show todo list
            std::vector<std::string> vs = get_todolist(CONF_PATH + "todo_list");
            TodoNotes todolist(WidgetProperty(WidgetProperty::TOP_RIGHT), vs);
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
        case 2:
        {
            // Finish task, move to archive
            // Select item
            std::vector<std::string> vs = get_todolist(CONF_PATH + "todo_list");
            TodoNotes todolist(WidgetProperty(WidgetProperty::TOP_RIGHT), vs);
            int id = todolist.one_touch_control();
            std::string dname = todolist.get_current_item_name(id);

            // Delete item and overwrite file with todo
            vs.erase(vs.begin() + id);
            std::ofstream ftodo;
            ftodo.open(CONF_PATH + "todo_list");
            for (auto& s : vs) 
            {
                ftodo << s << std::endl;
            }
            ftodo.close();

            // Archive deleted item
            ftodo.open(CONF_PATH + "todo_archive", std::ios_base::app);
            ftodo << dname << " | " << get_current_date() << std::endl;
            ftodo.close();
            print_status_bar(wp.screenCol, wp.screenRow, COLR_HIGH, "Todo is finished!");
            break;
        }
        case 3:
        {
            // Delete todo task
            // Select item
            std::vector<std::string> vs = get_todolist(CONF_PATH + "todo_list");
            TodoNotes todolist(WidgetProperty(WidgetProperty::TOP_RIGHT), vs);
            int id = todolist.one_touch_control();
            std::string dname = todolist.get_current_item_name(id);

            // Delete item and overwrite file with todo
            vs.erase(vs.begin() + id);
            std::ofstream ftodo;
            ftodo.open(CONF_PATH + "todo_list");
            for (auto& s : vs) 
            {
                ftodo << s << std::endl;
            }
            ftodo.close();
            print_status_bar(wp.screenCol, wp.screenRow, COLR_HIGH, "Todo is deleted!");
            break;
        }
        case 4:
        {
            // Show archive
            std::vector<std::string> vs = get_todolist(CONF_PATH + "todo_archive");
            TodoNotes archive_todolist(WidgetProperty(WidgetProperty::TOP_RIGHT), vs);
            archive_todolist.control();
            break;
        }
    }

    return global_id;
}