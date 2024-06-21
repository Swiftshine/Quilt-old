#pragma once
#include <iostream>
#include <string>

namespace Quilt::Debug {
    template <typename T>
    void Print(const T& t) {
        std::cout << t;
        std::cout << "\n";
    }

    template <typename T, typename... Args>
    void Print(const T& t, const Args&... args) {
        std::cout << t;
        Print(args...);
    }

    template <typename T>
    void Error(const T& t) {
        std::cout << "Quilt Error: " << t;
        std::cout << "\n";
    }

    template <typename T, typename... Args>
    void Error(const T& t, const Args&... args) {
        std::cout << "Quilt Error: " << t;
        Print(args...);
    }

    template <typename T>
    void Exception(const T& t) {
        std::cout << "Quilt Exception: " << t;
        std::cout << "\n";
    }

    template <typename T, typename... Args>
    void Exception(const T& t, const Args&... args) {
        std::cout << "Quilt Exception: " << t;
        Print(args...);
    }

    static void Assert(bool condition, std::string msg) {
        if (!condition) {
            std::cout << "Quilt Assertion Failed: " << msg << "\n";
            std::abort();
        }
    }
}