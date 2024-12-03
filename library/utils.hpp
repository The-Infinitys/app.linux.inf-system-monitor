#pragma once
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <cmath>
#include <codecvt>
#include <iomanip>
#include <iostream>
#include <sstream>

void print(std::string str) {
    std::cout << str << std::endl;
    return;
}

unsigned int msleep(int millseconds) { return usleep(millseconds * 1000); }

std::string int_to_hex_str(int i) {
    std::stringstream ss;
    ss << std::hex << i;
    return ss.str();
}

std::string color_hsv(double hue, double saturation, double value) {
    // HSVをRGBに変換

    struct RGB16 {
        int red;
        int green;
        int blue;
    };
    if (saturation < 0 || saturation > 1 || value < 0 || value > 1) {
        return {0, 0, 0};  // Return black for invalid input
    }
    hue = fmodf(hue, 360.0f) / 60.0f;
    int i = static_cast<int>(floorf(hue));
    float f = hue - i;
    float p = value * (1.0f - saturation);
    float q = value * (1.0f - saturation * f);
    float t = value * (1.0f - saturation * (1.0f - f));
    float red, green, blue;
    switch (i) {
        case 0:
            red = value;
            green = t;
            blue = p;
            break;
        case 1:
            red = q;
            green = value;
            blue = p;
            break;
        case 2:
            red = p;
            green = value;
            blue = t;
            break;
        case 3:
            red = p;
            green = q;
            blue = value;
            break;
        case 4:
            red = t;
            green = p;
            blue = value;
            break;
        default:
            red = value;
            green = p;
            blue = q;
            break;
            // Case 5
    }
    RGB16 rgb_data;
    rgb_data.red = static_cast<int>(roundf(red * 255.0f));
    rgb_data.green = static_cast<int>(roundf(green * 255.0f));
    rgb_data.blue = static_cast<int>(roundf(blue * 255.0f));
    int r, g, b;
    r = rgb_data.red;
    g = rgb_data.green;
    b = rgb_data.blue;
    // RGBを16進数のカラーコードに変換
    std::string color_code = "#";
    std::string r_str, g_str, b_str;
    r_str = int_to_hex_str(r);
    if (r_str.length() == 1) {
        r_str = "0" + r_str;
    }
    g_str = int_to_hex_str(g);
    if (g_str.length() == 1) {
        g_str = "0" + g_str;
    }
    b_str = int_to_hex_str(b);
    if (b_str.length() == 1) {
        b_str = "0" + b_str;
    }
    color_code += r_str + g_str + b_str;
    return color_code;
}
namespace keyboard {
int check_input(void) {
    struct termios oldt, newt;
    int ch0, ch1, ch2, ch3, ch4;
    int ret, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_iflag = ~(BRKINT | ISTRIP | IXON);
    newt.c_lflag = ~(ICANON | IEXTEN | ECHO | ECHOE | ECHOK | ECHONL);
    newt.c_cc[VTIME] = 0; /* キャラクタ間タイマを使わない */
    newt.c_cc[VMIN] = 1; /* 1文字来るまで，読み込みをブロックする */
    // newt.c_cc[VINTR]    = 0;     /* Ctrl-c */
    // newt.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
    // newt.c_cc[VERASE]   = 0;     /* del */
    // newt.c_cc[VKILL]    = 0;     /* @ */
    // newt.c_cc[VEOF]     = 4;     /* Ctrl-d */
    // newt.c_cc[VSWTC]    = 0;     /* '\0' */
    // newt.c_cc[VSTART]   = 0;     /* Ctrl-q */
    // newt.c_cc[VSTOP]    = 0;     /* Ctrl-s */
    // newt.c_cc[VSUSP]    = 0;     /* Ctrl-z */
    // newt.c_cc[VEOL]     = 0;     /* '\0' */
    // newt.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    // newt.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    // newt.c_cc[VWERASE]  = 0;     /* Ctrl-w */
    // newt.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
    // newt.c_cc[VEOL2]    = 0;     /* '\0' */
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) {
        fprintf(stderr, "error tcsetattr\n");
        exit(EXIT_FAILURE);
    }
    /* ノンブロックモードに設定 */
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (oldf < 0) {
        fprintf(stderr, "error fcntl\n");
        exit(EXIT_FAILURE);
    }
    if (fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK) < 0) {
        fprintf(stderr, "error fcntl\n");
        exit(EXIT_FAILURE);
    }

    ch0 = getchar();
    if (ch0 == 0x1B) {  // 矢印キー　Fnキーなどの取得　取得できないキーもある
        ch1 = getchar();
        ch2 = getchar();
        if (ch2 == 0x32) {
            ch3 = getchar();
            if (ch3 == 0x7e) {
                ret = (ch0 << 24) | (ch1 << 16) | (ch2 << 8) | ch3;
            } else {
                ch4 = getchar();
                ret = (ch1 << 24) | (ch2 << 16) | (ch3 << 8) | ch4;
            }
        } else if (ch2 == 0x31) {
            ch3 = getchar();
            ch4 = getchar();
            ret = (ch1 << 24) | (ch2 << 16) | (ch3 << 8) | ch4;
        } else if ((ch2 == 0x33) || (ch2 == 0x35) || (ch2 == 0x36)) {
            ch3 = getchar();
            ret = (ch0 << 24) | (ch1 << 16) | (ch2 << 8) | ch3;
        } else {
            ret = (ch0 << 16) | (ch1 << 8) | ch2;
        }
    } else if (ch0 != EOF) {
        ret = ch0;
    } else {
        ret = 0;
    }

    if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) == -1) {
        fprintf(stderr, "error tcsetattr\n");
        exit(EXIT_FAILURE);
    }
    if (fcntl(STDIN_FILENO, F_SETFL, oldf) < 0) {
        fprintf(stderr, "error fcntl\n");
        exit(EXIT_FAILURE);
    }
    if (ret == 0x03) {  //[Ctrl]+[C] で割り込み発生
        pid_t pid = getpid();
        kill(pid, SIGINT);
    }
    /*if(ret == 0x1a) { //[Ctrl]+[Z] で割り込み発生
        pid_t pid = getpid();
        kill(pid, SIGTSTP);
    }*/
    return ret;
}
}  // namespace key_input