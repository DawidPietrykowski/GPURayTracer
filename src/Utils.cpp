#include "Utils.h"

float GetRand() {
    return (float)((double)rand() / (RAND_MAX));
}

bool FileExists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}