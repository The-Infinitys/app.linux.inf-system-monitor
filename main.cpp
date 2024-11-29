#include "library/utils.hpp"
#include "library/terminal.hpp"

int main(int, char**) {
    terminal::clear();
    terminal::set_cursor_pos(1,1);
    print("Hello, from Infinity-task-manager!\n");
    sleep(1);
    terminal::clear();
    terminal::set_txt("Hello, World!",0,0);
    for (int i=0;i<10;i++){
        terminal::move_cursor_pos(1,1);
        terminal::write("うんち");
        terminal::renew();
        msleep(100);
    }
    terminal::clear();
    terminal::set_cursor_pos(0,0);
    return 0;
}
