#include "library/app.hpp"
#include "library/terminal.hpp"
#include "library/utils.hpp"

int main(int, char**) {
    terminal::clear();
    terminal::set_cursor_pos(1, 1);
    print("Hello, from Infinity-task-manager!\n");
    sleep(1);
    int ch;
    std::cout << "矢印キーを押してください (ESCで終了): ";
    terminal::set_cursor_pos(0, 0);
    while (true) {
        ch = keyboard::check_input();
        terminal::clear();
        application::program();
        terminal::set_txt(std::to_string(ch), 0, terminal::term_size()[0]);
        terminal::renew();
        msleep((int)(1000 / 30));
    }
    sleep(1);
    terminal::clear();
    terminal::set_cursor_pos(0, 0);
    return 0;
}
