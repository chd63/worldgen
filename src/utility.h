#ifndef UTILITY_H
#define UTILITY_H

// This was written with the use of gen ai

// This function is to get the base path
// of the executable file on windows and linux

#include <iostream>
#include <string>
#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <limits.h>
#endif

namespace fs = std::filesystem;

fs::path get_base_path() {
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return fs::path(buffer).parent_path();
#else
    char buffer[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
    if (count != -1) {
        return fs::path(std::string(buffer, count)).parent_path();
    }
    return fs::current_path(); // Fallback
#endif
}


#endif