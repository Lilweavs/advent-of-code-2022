#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include <set>

#include "clock.h"

using namespace std;

Timer timer;

int sumChars(vector<char> &vec) {
    int sum = 0;
    for (auto v : vec) {
        if (v >= 'a')
            sum += (v - 96);
        else
            sum += (v - 38);
    }   
    return sum;
}

// int tmp1[52], tmp2[52];

int main(int argc, char* argv[]) {

    timer.tick();

    ifstream ifile("input.txt", std::ios::in);

    string line;
    vector<char> rucksack;
    vector<char> intersection;

    while (getline(ifile, line, '\n')) {
        set<char> pack1(line.begin(), line.end() - line.length() / 2);
        set<char> pack2(line.begin() + line.length() / 2, line.end());
        set_intersection(pack1.begin(), pack1.end(), pack2.begin(), pack2.end(), back_inserter(intersection));
    }
    ifile.close();

    int ans1 = sumChars(intersection);

    vector<set<char>> rucksacks;
    ifile.open("input.txt", std::ios::in);
    while (getline(ifile, line, '\n')) {
        set<char> pack(line.begin(), line.end());
        rucksacks.push_back(pack);
    }
    ifile.close();

    vector<char> badges;
    for (int i = 0; i < static_cast<int>(rucksacks.size()); i+=3) {
        vector<char> tmp;
        set_intersection(rucksacks[i].begin(), rucksacks[i].end(), rucksacks[i+1].begin(), rucksacks[i+1].end(), back_inserter(tmp));
        set_intersection(tmp.begin(), tmp.end(), rucksacks[i+2].begin(), rucksacks[i+2].end(), back_inserter(badges));
    }

    int ans2 = sumChars(badges);

    timer.tock();

    cout << "Time Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << ans1 << endl;
    cout << "Part 2: " << ans2 << endl;

    return 0;
}   