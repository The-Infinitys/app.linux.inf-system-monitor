#pragma once
#include <iostream>
#include <vector>

#include "utils.hpp"

namespace terminal {
#define MAXLINE 1024
void write(std::string str) { std::cout << str; }
std::string cmd(std::string cmd) {
    FILE* fp;
    char buf[1024];
    std::string result = "";
    // コマンドを指定してパイプをオープン
    const char* cmd_char = cmd.c_str();
    if ((fp = popen(cmd_char, "r")) == NULL) {
        fprintf(stderr, "Failed to open pipe: argv[1]=%s", cmd_char);
        return "";
    }

    // パイプからのコマンド実行結果を受け取り表示
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        result += buf;
    }
    pclose(fp);
    return result;
}

void clear() { std::cout << "\e[2J"; }
void set_cursor_pos(int x, int y) {
    std::cout << "\e[" << y << ";" << x << "H";
}
void move_cursor_pos(int x, int y) {
    if (x > 0) {
        std::cout << "\e[" << (x) << "C";
    } else {
        std::cout << "\e[" << (-x) << "D";
    }
    if (y > 0) {
        std::cout << "\e[" << (y) << "B";
    } else {
        std::cout << "\e[" << (-y) << "A";
    }
}
void set_cursor_line(int line) {
    std::cout << "\e[" << (line) << ";" << (1) << "H";
}
void move_cursor_line(int line) {
    if (line < 0) {
        std::cout << "\e[" << (-line) << "F";
    } else {
        std::cout << "\e[" << (line) << "E";
    }
}
void next_line() { move_cursor_line(1); }
void set_txt(const std::string& text, int x, int y) {
    std::cout << "\e[" << y << ";" << x << "H" << text;
}
void renew() { std::cout.flush(); }
std::string color_txt_rgb(const std::string& str,
                          const std::string& hex_color) {
    std::ostringstream r;
    std::ostringstream g;
    std::ostringstream b;
    r << std::stoi(hex_color.substr(1, 2), nullptr, 16);  // 赤の16進数部分
    g << std::stoi(hex_color.substr(3, 2), nullptr, 16);  // 緑の16進数部分
    b << std::stoi(hex_color.substr(5, 2), nullptr, 16);  // 青の16進数部分
    // 16進数カラーコードのRGB値をターミナルカラーに変換
    return (std::string) "\e[38;2;" + r.str() + ";" + g.str() + ";" + b.str() +
           "m" + str + "\e[0m";
}
std::string color_txt_hsv(const std::string& str, double hue, double saturation,
                          double brightness) {
    return color_txt_rgb(str, color_hsv(hue, saturation, brightness));
}
std::string color_back_rgb(const std::string& str,
                           const std::string& hex_color) {
    std::ostringstream r;
    std::ostringstream g;
    std::ostringstream b;
    r << std::stoi(hex_color.substr(1, 2), nullptr, 16);  // 赤の16進数部分
    g << std::stoi(hex_color.substr(3, 2), nullptr, 16);  // 緑の16進数部分
    b << std::stoi(hex_color.substr(5, 2), nullptr, 16);  // 青の16進数部分
    // 16進数カラーコードのRGB値をターミナルカラーに変換
    return (std::string) "\e[48;2;" + r.str() + ";" + g.str() + ";" + b.str() +
           "m" + str + "\e[0m";
}
void draw_rect(int x, int y, int width, int height,
               const std::string& hex_color) {
    std::string emp_bar = "";
    for (int pos_x = 0; pos_x < width; pos_x++) {
        emp_bar += " ";
    }
    for (int pos_y = 0; pos_y < height; pos_y++) {
        set_txt(color_back_rgb(emp_bar, hex_color), x, y + pos_y);
    }
}
std::vector<int> term_size() {
    int terminal_width = stoi(cmd("tput cols"));
    int terminal_height = stoi(cmd("tput lines"));
    return {terminal_width, terminal_height};
}

}  // namespace terminal
