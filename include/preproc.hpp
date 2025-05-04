//
// Created by Alexander on 03.05.25.
//

#ifndef PREPROC_HPP
#define PREPROC_HPP
#include <string>
#include <unordered_map>


class Preproc {
    std::string& input; // the input is the main file which may include other files

public:
    explicit Preproc(std::string& input) : input(input) {}

    void resolveAllIncludes();
    void resolveAllMacros();

private:
    bool handleInclusions();
    bool handleMacros();

    static std::string normalizeWhitespace(std::string &code);

    std::string unescape(const std::string& str);

    std::unordered_map<std::string, std::string> macros;
};



#endif //PREPROC_HPP
