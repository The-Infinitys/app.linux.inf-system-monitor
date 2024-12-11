#pragma once
#include "term-gui.hpp"
#include "terminal.hpp"
#include "utils.hpp"
namespace application {
int title_color = 0;
int term_lines = 0;
std::vector<std::string> get_machine_process_info() {
    return string_split(terminal::cmd("top -b -n 1"), '\n');
    ;
}

void program() {
    term_lines = 1;
    title_color += 1;
    title_color %= 360;
    std::vector<int> term_info = terminal::term_size();
    terminal::set_txt(terminal::color_txt_hsv("The Infinity's task manager",
                                              title_color, 1, 1),
                      2, term_lines);
    term_lines++;
    std::vector<std::string> process_info = get_machine_process_info();
    for (int i = 0; i < process_info.size(); i++) {
        terminal::set_txt(process_info[i], 2, term_lines);
        ++term_lines;
    }

    terminal::draw_col(0, color_hsv(title_color, 1, 1));
}
}  // namespace application