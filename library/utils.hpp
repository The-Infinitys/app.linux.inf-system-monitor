#pragma once
#include <stdio.h>
#include <string.h>
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
    double c = value * saturation;
    double h = hue / 60.0;
    double x = c * (1 - abs(fmod(h, 2) - 1));
    double m = value - c;

    int r, g, b;
    if (0 <= h && h < 1) {
        r = c;
        g = x;
        b = 0;
    } else if (1 <= h && h < 2) {
        r = x;
        g = c;
        b = 0;
    } else if (2 <= h && h < 3) {
        r = 0;
        g = c;
        b = x;
    } else if (3 <= h && h < 4) {
        r = 0;
        g = x;
        b = c;
    } else if (4 <= h && h < 5) {
        r = x;
        g = 0;
        b = c;
    } else {
        r = c;
        g = 0;
        b = x;
    }
    r = (int)(r * 255 + 0.5);
    g = (int)(g * 255 + 0.5);
    b = (int)(b * 255 + 0.5);

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