#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>

#include "clock.h"

using namespace std;

Timer timer;

map<pair<char,char>, pair<int,int>> possibilties = {
    {pair('A','X'), pair(1 + 3, 3 + 0)},
    {pair('A','Y'), pair(2 + 6, 1 + 3)},
    {pair('A','Z'), pair(3 + 0, 2 + 6)},
    {pair('B','X'), pair(1 + 0, 1 + 0)},
    {pair('B','Y'), pair(2 + 3, 2 + 3)},
    {pair('B','Z'), pair(3 + 6, 3 + 6)},
    {pair('C','X'), pair(1 + 6, 2 + 0)},
    {pair('C','Y'), pair(2 + 0, 3 + 3)},
    {pair('C','Z'), pair(3 + 3, 1 + 6)}};

int main(int argc, char* argv[]) {

    timer.tick();

    ifstream ifile("input.txt", std::ios::in);

    string line;
    vector<pair<char,char>> round;
    string playerOne, playerTwo;
    while (getline(ifile, line, '\n')) {
        round.push_back(pair(line[0], line[2]));
    }
    ifile.close();

    int ans1 = 0, ans2 = 0;
    for (const auto& r : round) {
        auto [i, j] = possibilties[r];
        ans1 += i;
        ans2 += j;
    }

    timer.tock();

    cout << "Time Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << ans1 << endl;
    cout << "Part 2: " << ans2 << endl;

    return 0;
}   