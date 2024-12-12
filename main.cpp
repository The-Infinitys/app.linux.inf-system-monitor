#include "library/app.hpp"
#include "library/terminal.hpp"
#include "library/utils.hpp"

int main(int, char**) {
    terminal::clear();
    terminal::set_cursor_pos(1, 1);
    std::cout << manager::info().task.total << std::endl;
    std::cout << manager::info().task.running << std::endl;
    std::cout << manager::info().task.sleeping << std::endl;
    std::cout << manager::info().task.stopped << std::endl;
    std::cout << manager::info().task.zombie << std::endl;
    sleep(1);
    print("Hello, from Infinity-task-manager!\n");
    sleep(1);
    int ch;
    std::cout << "矢印キーを押してください (ESCで終了): ";
    terminal::set_cursor_pos(0, 0);
    while (true) {
        terminal::clear();
        application::program();
        terminal::renew();
        msleep((int)(1000 / 30));
    }
    sleep(1);
    terminal::clear();
    terminal::set_cursor_pos(0, 0);
    return 0;
}
