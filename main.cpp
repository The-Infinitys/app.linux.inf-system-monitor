#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <vector>

#define MAXLINE 1024

void print(std::string str) {
    std::cout << str << std::endl;
    return;
}

unsigned int msleep(int millseconds) { return usleep(millseconds * 1000); }

std::string exec_command(std::string cmd) {
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

void clear() {
    std::cout << "\033[2J";
    std::cout.flush();
}
void set_txt(const std::string& text, int x, int y) {
    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H" << text;
}

void renew() { std::cout.flush(); }
std::string color_txt(const std::string& str, const std::string& hex_color) {
    std::ostringstream r;
    std::ostringstream g;
    std::ostringstream b;
    r << std::stoi(hex_color.substr(1, 2), nullptr, 16);  // 赤の16進数部分
    g << std::stoi(hex_color.substr(3, 2), nullptr, 16);  // 緑の16進数部分
    b << std::stoi(hex_color.substr(5, 2), nullptr, 16);  // 青の16進数部分
    // 16進数カラーコードのRGB値をターミナルカラーに変換
    return (std::string) "\033[38;2;" + r.str() + ";" + g.str() + ";" +
           b.str() + "m" + str + "\033[0m";
}

std::string color_back(const std::string& str, const std::string& hex_color) {
    std::ostringstream r;
    std::ostringstream g;
    std::ostringstream b;
    r << std::stoi(hex_color.substr(1, 2), nullptr, 16);  // 赤の16進数部分
    g << std::stoi(hex_color.substr(3, 2), nullptr, 16);  // 緑の16進数部分
    b << std::stoi(hex_color.substr(5, 2), nullptr, 16);  // 青の16進数部分
    // 16進数カラーコードのRGB値をターミナルカラーに変換
    return (std::string) "\033[48;2;" + r.str() + ";" + g.str() + ";" +
           b.str() + "m" + str + "\033[0m";
}
void draw_window(int x, int y, int width, int height,
                 const std::string& hex_color) {
    std::string emp_bar = "";
    for (int pos_x = 0; pos_x < width; pos_x++) {
        emp_bar += " ";
    }
    for (int pos_y = 0; pos_y < height; pos_y++) {
        set_txt(color_back(emp_bar, hex_color), x, y + pos_y);
    }
}
std::vector<int> get_terminal_size() {
    int terminal_width = stoi(exec_command("tput cols"));
    int terminal_height = stoi(exec_command("tput lines"));
    return {terminal_width, terminal_height};
}
int main(int, char**) {
    print("Hello, from Infinity-task-manager!\n");
    for (int count = 0; count < 100; count++) {
        clear();
        std::vector<int> terminal_size = get_terminal_size();
        int terminal_width = terminal_size[0];
        int terminal_height = terminal_size[1];
        draw_window(0, 0, terminal_width, terminal_height, "#333333");
        for (int i = 0; i < terminal_height; i++) {
            set_txt(color_back(color_txt("Hello!", "#ff0000"), "#333333"), i,
                    i);
            set_txt(color_back("World!", "#333333"), i + 20, i);
        }
        set_txt("", terminal_width, terminal_height);  // カーソルを退避させる
        renew();
        msleep(100);
    }

    clear();
    set_txt("", 0, 0);
    return 0;
}
