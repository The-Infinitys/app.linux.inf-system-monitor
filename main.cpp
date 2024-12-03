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
        terminal::clear();
        terminal::set_txt(terminal::color_txt_hsv("Hello!", (double)i, 1, 1),
                          i % 5, 1);
        terminal::renew();
        msleep((int)(1000 / 30));
    }
    sleep(1);
    terminal::clear();
    terminal::set_cursor_pos(0, 0);
    return 0;
}
