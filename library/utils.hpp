#pragma once
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <codecvt>
#include <iostream>
#include <sstream>

void print(std::string str) {
    std::cout << str << std::endl;
    return;
}

unsigned int msleep(int millseconds) { return usleep(millseconds * 1000); }

int count_byte(char c) {
    if ((c & 0x80) == 0) {
        return 1;
    }
    if ((c & 0xe0) == 0xc0) {
        return 2;
    }
    if ((c & 0xf0) == 0xe0) {
        return 3;
    }
    return 4;
}

int count_str(std::string string) {
    char* strings = (char[])string.c_str();
    int count = 0;
    char c = *strings;
    while (c != '\0') {
        count += 1;
        strings += count_byte(c);
        c = *strings;
    }
    return count;
}
// TODO: ここを治す