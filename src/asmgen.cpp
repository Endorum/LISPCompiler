//
// Created by Alexander on 08.05.25.
//

#include "../include/asmgen.hpp"

std::string Asmgen::cleanup() {
        
        std::istringstream iss(asm_result);
        std::vector<std::string> lines;
        std::string line;

        // Read all lines into a vector
        while (std::getline(iss, line)) {
            lines.push_back(line);
        }

        std::vector<std::string> cleaned_lines;

        // Regex for mov instruction with two operands
        std::regex mov_pattern(R"(^\s*mov\s+([^,]+),\s*(.+)\s*$)", std::regex::icase);

        for (size_t i = 0; i < lines.size(); ++i) {
            if (i + 1 < lines.size()) {
                std::smatch m1, m2;

                if (std::regex_match(lines[i], m1, mov_pattern) && std::regex_match(lines[i + 1], m2, mov_pattern)) {
                    std::string dst1 = m1[1].str();
                    std::string src1 = m1[2].str();

                    std::string dst2 = m2[1].str();
                    std::string src2 = m2[2].str();

                    // Trim whitespace from operands (optional but recommended)
                    auto trim = [](std::string& s) {
                        size_t start = s.find_first_not_of(" \t");
                        size_t end = s.find_last_not_of(" \t");
                        if (start == std::string::npos) s = "";
                        else s = s.substr(start, end - start + 1);
                    };

                    trim(dst1);
                    trim(src1);
                    trim(dst2);
                    trim(src2);

                    if (dst1 == src2 && src1 == dst2) {
                        // Found matching mov X, Y and mov Y, X pair — skip both lines
                        i++;
                        continue;
                    }
                }
            }

            cleaned_lines.push_back(lines[i]);
        }

        std::ostringstream oss;
        for (auto& l : cleaned_lines) {
            oss << l << "\n";
        }

        asm_result = oss.str();
        return asm_result;
    }