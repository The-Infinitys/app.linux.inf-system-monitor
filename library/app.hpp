#pragma once
#include "term-gui.hpp"
#include "terminal.hpp"
#include "utils.hpp"
namespace application {
int title_color = 0;
void program() {
    title_color += 1;
    title_color %= 360;
    terminal::set_txt(terminal::color_txt_hsv("The Infinity's task manager",
                                              title_color, 1, 1),
                      0, 0);
}
}  // namespace application