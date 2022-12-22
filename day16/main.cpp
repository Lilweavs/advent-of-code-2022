#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>
#include <unordered_map>

#include "clock.h"

using namespace std;

Timer timer;

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<array<int,3>> instructions;
    array<string,9> cargo;

    ifstream ifile("input.txt", std::ios::in);

    unordered_map<string, pair<int,vector<string>>> graph;

    regex rxValve("[A-Z]{2}");
    regex rxNum("\\d+");
    while (getline(ifile, line, '\n')) {

        smatch match;
        regex_search(line, match, rxNum);
        int valveRate = stoi(match.str());

        sregex_iterator it(line.begin(), line.end(), rxValve);
        int i = 0;
        string valve = "";
        while (it != sregex_iterator()) {
            match = *it++;
            if (0 == i++) {
                valve = match.str();
                graph.insert({match.str(), pair<int,vector<string>>(valveRate,vector<string>())});
            } else {
                graph.at(valve).second.push_back(match.str());
            }
        }
    }
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    



    // cout << "Part 1: " << part1(instructions, cargo) << endl;
    // cout << "Part 2: " << part2(instructions, cargo) << endl;

    return 0;
}