#pragma once
#include <iostream>

#define LOG_ERROR(msg) \
    std::cout << '[' << __TIME__ << "] ERROR: " << msg << std::endl

float GetRand();
bool FileExists(const std::string& name);