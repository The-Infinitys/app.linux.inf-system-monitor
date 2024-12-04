#pragma once
#include "term-gui.hpp"
#include "terminal.hpp"
#include "utils.hpp"
namespace application {
int title_color = 0;
void draw_cpu_usage() {
    std::vector<std::string> mpstat_data =
        string_split(terminal::cmd("mpstat -u -P ALL"), '\n');
    std::string cpu_name = mpstat_data[0];
    int idle_data_position = mpstat_data[2].find("idle") - 1;
    terminal::set_txt(
        terminal::color_txt_rgb(
            "CPU: " + cpu_name.substr(0, cpu_name.find("\t")), "#00ff00"),
        1, 2);
    std::vector<double> cpu_usage_info(mpstat_data.size() - 3, 0);
    for (int i = 3; i < mpstat_data.size(); i++) {
        double usage_var =
            100.0 - std::stod(mpstat_data[i].substr(idle_data_position,
                                                    mpstat_data[i].length()));
        cpu_usage_info[i - 3] = usage_var;
    }
    for (int i = 0; i < cpu_usage_info.size(); i++) {
        if (i == 0) {
            terminal::set_txt(
                "ALL-CPU: " + std::to_string(cpu_usage_info[i]) + "%", 1,
                i + 4);
        } else {
            terminal::set_txt("CPU-" + std::to_string(i) + ": " +
                                  std::to_string(cpu_usage_info[i]) + "%",
                              1, i + 4);
        }
    }
}
void program() {
    title_color += 1;
    title_color %= 360;
    terminal::set_txt(terminal::color_txt_hsv("The Infinity's task manager",
                                              title_color, 1, 1),
                      0, 0);
    draw_cpu_usage();

}  // namespace application