#pragma once
#include "term-gui.hpp"
#include "terminal.hpp"
#include "utils.hpp"
namespace application {
int title_color = 0;
int term_lines = 0;
void draw_cpu_usage() {
    terminal::draw_line(2, "#00ff00");
    std::vector<std::string> mpstat_data =
        string_split(terminal::cmd("mpstat -u -P ALL"), '\n');
    std::string cpu_name = mpstat_data[0];
    int idle_data_position = mpstat_data[2].find("idle") - 1;
    terminal::set_txt(
        terminal::color_txt_rgb(
            "CPU: " + cpu_name.substr(0, cpu_name.find("\t")), "#00ff00"),
        2, 3);
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
                "ALL-CPU: " + std::to_string(cpu_usage_info[i]) + "%", 2,
                i + 4);
        } else {
            terminal::set_txt("CPU-" + std::to_string(i) + ": " +
                                  std::to_string(cpu_usage_info[i]) + "%",
                              2, i + 4);
        }
    }
    term_lines += cpu_usage_info.size() + 4;
}
void draw_process_info() {
    terminal::draw_line(term_lines, "#ffff00");
    term_lines++;
    std::vector<std::string> process_info_data =
        string_split(terminal::cmd("top -b -n 1"), '\n');
    ;
    int term_height = terminal::term_size()[1];
    bool is_general_info = false;
    int data_count = 0;
    while (term_height > term_lines) {
        if (process_info_data[data_count] == "") {
            is_general_info = true;
        }
        if (is_general_info) {
            terminal::set_txt(
                terminal::color_txt_rgb(process_info_data[data_count], "#005500"), 2,
                term_lines);
        } else {
            terminal::set_txt(process_info_data[data_count], 2, term_lines);
        }
        data_count++;
        term_lines++;
    }
}
void program() {
    term_lines = 0;
    title_color += 1;
    title_color %= 360;
    std::vector<int> term_info = terminal::term_size();
    terminal::set_txt(terminal::color_txt_hsv("The Infinity's task manager",
                                              title_color, 1, 1),
                      2, 0);
    draw_cpu_usage();
    draw_process_info();
    terminal::draw_col(0, color_hsv(title_color, 1, 1));
}
}  // namespace application