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
std::vector<std::string> string_split(std::string str, char del) {
    int first = 0;
    int last = str.find_first_of(del);
    std::vector<std::string> result;
    while (first < str.size()) {
        std::string subStr(str, first, last - first);
        result.push_back(subStr);
        first = last + 1;
        last = str.find_first_of(del, first);
        if (last == std::string::npos) {
            last = str.size();
        }
    }
    return result;
}
