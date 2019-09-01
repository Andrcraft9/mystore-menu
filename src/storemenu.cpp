#include "mstor/storemenu.h"

StoreMenu::StoreMenu(WidgetProperty wp) : Menu(wp, {"MyStore, Create", "MyStore, Read", "MyStore, Archive", "TODO"}) 
{
}

int StoreMenu::action(int global_id)
{
    namespace fs = std::filesystem;

    switch(global_id)
    {
        case 0:
        {
            // Create note and crypt it
            char crypt[CRYPT_PSWD_MAX_LENGTH];
            def_prog_mode(); /* Save the tty modes */
            endwin(); /* End curses mode temporarily */
            std::system("nano note");

            printf("Note is written! \n");
            char answer;
            printf("Do you want save it? (y/n) \n");
            std::cin >> answer;
            if (answer == 'y')
            {
                printf("Please enter password: \n");
                scanf("%s", crypt);
                std::string namef = STOR_PATH + get_current_date();
                write_store(crypt, "./note", namef);
            }
            std::system("rm note");

            reset_prog_mode(); /* Return to the previous tty mode*/
            
            if (answer == 'y')
                print_status_bar(wp.screenCol, wp.screenRow, COLR_HIGH, "Note is MyStored!");
            else
                print_status_bar(wp.screenCol, wp.screenRow, COLR_HIGH, "Note is deleted!");

            break;
        }
        case 1:
        {
            // Read note
            char crypt[CRYPT_PSWD_MAX_LENGTH];
            def_prog_mode();
            endwin();
            printf("Please enter password: \n");
            scanf("%s", crypt);
            reset_prog_mode();

            std::string path = STOR_PATH;
            std::vector<std::tuple<std::string, std::time_t> > vstime;
            for (const auto & p : fs::directory_iterator(path))
            {
                auto ftime = fs::last_write_time(p);
                std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime); 
                vstime.push_back(std::make_tuple(p.path().u8string(), cftime));
            }
            // Sort by date
            std::sort(vstime.begin(), vstime.end(), sortbydate);
            // Obtain only names from sorted vector of tuples
            std::vector<std::string> vs;
            for(auto& it : vstime)
                vs.push_back(std::get<0>(it));

            StoreNotes store_notes(WidgetProperty(WidgetProperty::BOTTOM_LEFT), vs, crypt);
            store_notes.control();
            break;
        }
        case 2:
        {
            // Crypt created note
            char crypt[CRYPT_PSWD_MAX_LENGTH];
            def_prog_mode(); /* Save the tty modes */
            endwin(); /* End curses mode temporarily */

            std::string ifilename, ofilename;
            printf("Print full path of input file: \n");
            std::cin >> ifilename;
            printf("Print name of output file: \n");
            std::cin >> ofilename;

            char answer;
            printf("Do you want archive %s to %s? (y/n) \n", ifilename.c_str(), ofilename.c_str());
            std::cin >> answer;
            if (answer == 'y')
            {
                printf("Please enter password: \n");
                scanf("%s", crypt);
                if (write_store(crypt, ifilename, STOR_PATH + ofilename) == 0)
                    std::system((std::string("rm ") + ifilename).c_str());
                else
                {
                    printf("Error in write_store! \n");
                    getch();
                }
            }

            reset_prog_mode(); /* Return to the previous tty mode*/
            
            if (answer == 'y')
                print_status_bar(wp.screenCol, wp.screenRow, COLR_HIGH, "Note is archived!");
            else
                print_status_bar(wp.screenCol, wp.screenRow, COLR_HIGH, "Note is ignored");
        
            break;
        }
        case 3:
        {
            // Show TODO menu
            TodoMenu todo_menu(WidgetProperty(WidgetProperty::TOP_MIDDLE));
            todo_menu.control();
            break;
        }
    }

    return global_id;
}