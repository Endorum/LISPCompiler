//
// Created by Alexander on 03.05.25.
//

#include "../include/preproc.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

#define MAX_INCLUDE_DEPTH 10
#define MAX_MACRO_DEPTH 10

bool Preproc::handleInclusions() {
    std::stringstream processed;
    std::istringstream inputStream(this->input);
    std::string line;

    std::regex includePattern(R"(^\s*#include\s+"(.+)\"\s*$)");

    bool includesLeft = false;

    while (std::getline(inputStream, line)) {
        std::smatch match;
        if (std::regex_match(line, match, includePattern)) {
            std::string includePath = match[1];
            std::ifstream file(includePath);
            if (!file.is_open()) {
                std::cerr << "Error: Could not open included file '" << includePath + "'\n";
                exit(1);
            }
            includesLeft = true;
            std::string includedContent((std::istreambuf_iterator<char>(file)),
                                         std::istreambuf_iterator<char>());
            processed << includedContent << "\n";
        } else {
            processed << line << "\n";
        }
    }

    this->input = processed.str();
    return includesLeft;
}

bool Preproc::handleMacros() {
    std::regex definePattern(R"(#define\s+(\w+)\s+"((?:[^"\\]|\\.)*?)\")");
    std::smatch match;
    std::string updated;
    std::string::const_iterator searchStart(input.cbegin());

    bool changed = false;

    // 1. Extract all macro definitions
    while (std::regex_search(searchStart, input.cend(), match, definePattern)) {
        std::string name = match[1];
        std::string rawValue = match[2];
        std::string value = unescape(rawValue);

        // Store macro
        macros[name] = value;
        changed = true;

        // Remove this #define line from the input
        size_t matchStart = match.position(0);
        input.erase(matchStart, match.length(0));
        searchStart = input.cbegin() + matchStart;
    }

    // 2. Replace macros in the entire input string
    for (const auto& [key, val] : macros) {
        size_t pos = 0;
        while ((pos = input.find(key, pos)) != std::string::npos) {
            input.replace(pos, key.length(), val);
            pos += val.length();
            changed = true;
        }
    }

    return changed;
}

std::string Preproc::normalizeWhitespace(std::string &code) {
    // Variable to track the position while iterating
    // size_t pos = 0;
    //
    // // Replace consecutive newlines with a single space
    // while ((pos = code.find("\n\n", pos)) != std::string::npos) {
    //     code.replace(pos, 2, " ");  // Replace "\n\n" with a single "\n"
    //     pos += 1;  // Move past the replaced newline
    // } //TODO: the problem is that if the line above is a comment, then the line below might "become" a comment as the line break is removed


    return code;
}




std::string Preproc::unescape(const std::string& str) {
    std::string result;
    bool escape = false;

    for (int i=0;i<str.size();i++) {
        char c = str.at(i);
        if (escape) {
            if (c == 'n') result += '\n';
            else if (c == 't') result += '\t';
            else result += c;
            escape = false;
        } else if (c == '\\') {
            escape = true;
        } else {
            result += c;
        }
    }
    return result;
}



void Preproc::resolveAllIncludes() {
    int depth = 0;
    while (handleInclusions()) {
        depth++;
        if (depth > MAX_INCLUDE_DEPTH) {
            std::cerr << "Error: Max include depth (" << MAX_INCLUDE_DEPTH << ") exceeded\n";
            exit(1);
        }
    }
}

void Preproc::resolveAllMacros() {
    int depth = 0;
    while (handleMacros()) {
        depth++;
        if (depth > MAX_MACRO_DEPTH) {
            std::cerr << "Error: Max macro resolution depth (" << MAX_MACRO_DEPTH << ") exceeded\n";
            exit(1);
        }
    }

    this->input = normalizeWhitespace(this->input);;
}
