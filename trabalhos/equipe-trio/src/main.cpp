#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../include/sorted_list.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " input_file\n";
        return 1;
    }
    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "could not open " << argv[1] << "\n";
        return 1;
    }

    SortedList list;
    std::string line;
    while (std::getline(input, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        std::istringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "INC") {
            int n;
            if (ss >> n) list.insert(n);
        } else if (cmd == "REM") {
            int n;
            if (ss >> n) list.remove(n);
        } else if (cmd == "SUC") {
            int n, m;
            if (ss >> n >> m) {
                int s = list.successor(n, m);
                std::cout << line << "\n";
                if (s >= 0) std::cout << s << "\n";
                else std::cout << "inf\n";
            }
        } else if (cmd == "IMP") {
            int m;
            if (ss >> m) std::cout << line << "\n" << list.print(m) << "\n";
        }
    }
    return 0;
}
