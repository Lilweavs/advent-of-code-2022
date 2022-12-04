#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include <set>
#include <array>

#include "clock.h"

#define CHAR_RANGE 'z' - 'A'

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

    /* Part 1 New */
    timer.tick();
    int intersectionSum = 0;
    for (const auto &rs : rucksack) {
        uint64_t freq1 = 0, freq2 = 0;
        
        int middle = rs.length() / 2;
        for (int i = 0; i < middle; i++) {
            freq1 |= (static_cast<uint64_t>(1) << (rs[i] - 'A'));
            freq2 |= (static_cast<uint64_t>(1) << (rs[i+middle] - 'A'));
        }

        uint64_t intersection = freq1 & freq2;
        int i = 0;
        while (((intersection >> i++) & 1) != 1);
        intersectionSum += table[--i + 'A'];
    }
    
    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << intersectionSum << endl; 

    /* Part 2 New */
    timer.tick();
    intersectionSum = 0;
    uint64_t intersection = UINT64_MAX;
    for (size_t i = 0; i < rucksack.size(); i+=3) {
        uint64_t freq1 = 0;

        for (size_t j = i; j < i+3; j++) {
            for (const auto c : rucksack[j])
                freq1 |= (static_cast<uint64_t>(1) << (c - 'A'));
            intersection &= freq1;
            freq1 = 0; 
        }

        int j = 0;
        while (((intersection >> j++) & 1) != 1);
        intersectionSum += table[--j + 'A'];
        intersection = UINT64_MAX;
    }

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << intersectionSum << endl;

    return 0;
}   