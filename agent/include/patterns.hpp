#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

inline std::vector<std::string> load_patterns(const std::string& filepath = "./patterns.txt") {
    std::vector<std::string> patterns;
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Warning: patterns file '" << filepath << "' not found. Using default patterns.\n";
        patterns = {
            "unauthorized access",
            "failed login",
            "invalid user",
            "segfault",
            "buffer overflow",
            "connection refused",
            "access denied",
            "root access",
            "port scan",
            "malware detected"
        };
        return patterns;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            patterns.push_back(line);
        }
    }

    if (patterns.empty()) {
        std::cerr << "Warning: patterns file '" << filepath << "' is empty. Using default patterns.\n";
        patterns = {
            "unauthorized access",
            "failed login",
            "invalid user",
            "segfault",
            "buffer overflow",
            "connection refused",
            "access denied",
            "root access",
            "port scan",
            "malware detected"
        };
    }
    return patterns;
}