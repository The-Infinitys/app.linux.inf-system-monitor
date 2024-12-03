#include "library/terminal.hpp"
#include "library/utils.hpp"

int main(int, char**) {
    terminal::clear();
    terminal::set_cursor_pos(1, 1);
    print("Hello, from Infinity-task-manager!\n");
    sleep(1);
    terminal::clear();
    terminal::set_txt("Hello, World!", 0, 0);
    terminal::clear();
    terminal::set_cursor_pos(0, 0);
    for (int i = 0; i < 360; ++i) {
        print(color_hsv(i, 0.5, 0.5));
        msleep((int)(1000 / 30));
    }
    sleep(1);
    return 0;
}

