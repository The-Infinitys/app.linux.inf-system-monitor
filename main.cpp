#include "library/utils.hpp"
#include "library/terminal.hpp"

int main(int, char**) {
    terminal::clear();
    terminal::set_cursor_pos(1,1);
    print("Hello, from Infinity-task-manager!\n");
    sleep(1);
    terminal::clear();
    terminal::set_txt("Hello, World!",0,0);
    terminal::clear();
    terminal::set_cursor_pos(0,0);
    return 0;
}
