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
