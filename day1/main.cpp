#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>

#include "clock.h"

using namespace std;

Timer timer;

// Test comment
// what
int main(int argc, char* argv[]) {

    timer.tick();

    ifstream ifile("input.txt", std::ios::in);

    string line;
    vector<int> calories;
    int sum = 0;
    while (getline(ifile, line)) {
        if (!line.empty()) {
            sum += stoi(line);    
        } else {
            calories.push_back(sum);
            sum = 0;
        }
    }
    ifile.close();

    sort(calories.rbegin(), calories.rend());

    int ans1 = calories[0];
    int ans2 = calories[0] + calories[1] + calories[2];
    
    cout << "Time Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << ans1 << endl;
    cout << "Part 2: " << ans2 << endl;

    return 0;
}   