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
        array<int, CHAR_RANGE> freq1 = {0}, freq2 = {0};
        int middle = rs.length() / 2;
        for (int i = 0; i < middle; i++) {
            freq1[rs[i]-'A'] += 1;
            freq2[rs[i+middle]-'A'] += 1;
        }

        for (size_t i = 0; i <= freq1.size(); i++) {
            if (min(freq1[i], freq2[i])) {
                intersectionSum += table[i + 'A'];
                break;
            }
        }
    }
    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << intersectionSum << endl; 

    /* Part 2 New */
    timer.tick();
    intersectionSum = 0;
    for (size_t i = 0; i < rucksack.size(); i+=3) {
        array<int, CHAR_RANGE> freq1 = {0}, freq2 = {0}, freq3 = {0};
        // for (size_t j = 0; j < max({rucksack[i].size(), rucksack[i+1].size(), rucksack[i+2].size()}); j++) {
        //     if (j < rucksack[i].size())
        //         freq1[rucksack[i][j] - 'A']   += 1;
        //     if (j < rucksack[i+1].size())
        //         freq2[rucksack[i+1][j] - 'A'] += 1;
        //     if (j < rucksack[i+2].size())
        //         freq3[rucksack[i+2][j] - 'A'] += 1;
        // }
        for (auto& c : rucksack[i])
            freq1[c - 'A'] += 1;
        for (auto& c : rucksack[i+1])
            freq2[c - 'A'] += 1;
        for (auto& c : rucksack[i+2])
            freq3[c - 'A'] += 1;
    
        for (size_t j = 0; j <= freq1.size(); j++) {
            if (min({freq1[j], freq2[j], freq3[j]})) {
                intersectionSum += table[j + 'A'];
                break;
            }
        }
    }

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << intersectionSum << endl;

    return 0;
}   