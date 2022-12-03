#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include <set>
#include <array>

#include "clock.h"

using namespace std;

Timer timer;

constexpr array<int, 128> MakeTable() {
    array<int, 128> arr{};
    for (int i = 'A'; i <= 'Z'; i++)
        arr[i] = i - 38;
    for (char i = 'a'; i <= 'z'; i++)
        arr[i] = i - 96;
    return arr;
}

constexpr auto table = MakeTable();

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<string> rucksack;
    ifstream ifile("input.txt", std::ios::in);
    while (getline(ifile, line, '\n')) {
        rucksack.push_back(line);
    }
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    /* Part 1 Original Optimized */
    timer.tick();
    vector<char> common;
    int intersectionSum = 0;
    for (auto& rs: rucksack) {
        int middle = rs.length() / 2;
        sort(rs.begin(), rs.end() - middle);
        sort(rs.begin() + middle, rs.end());
        set_intersection(rs.begin(), rs.end() - middle, rs.begin() + middle, rs.end(), back_inserter(common));
        intersectionSum += table[common[0]];
        common.clear();
    }
    
    cout << "Time Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << intersectionSum << endl;
    
    /* Part 2 Old Optimized */
    timer.tick();
    for (auto& rs : rucksack)
        sort(rs.begin(), rs.end());

    intersectionSum = 0;
    for (size_t i = 0; i < rucksack.size(); i+=3) {
        set_intersection(rucksack[i].begin(), rucksack[i].end(), rucksack[i+1].begin(), rucksack[i+1].end(), back_inserter(common));
        set_intersection(common.begin(), common.end(), rucksack[i+2].begin(), rucksack[i+2].end(), back_inserter(common));
        intersectionSum += table[common.back()];
        common.clear();
    }

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << intersectionSum << endl;

    return 0;
}   