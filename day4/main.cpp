#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>

#include "clock.h"

using namespace std;

Timer timer;

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<array<int,4>> pairs;
    ifstream ifile("input.txt", std::ios::in);
    regex rx("\\d+");
    while (getline(ifile, line, '\n')) {
        
        array<int, 4> arr;
        sregex_iterator it(line.begin(), line.end(), rx);
        int i = 0;
        while (it != sregex_iterator()) {
            smatch match = *it++;
            arr[i++] = stoi(match.str());   
        }
        pairs.push_back(arr);
    }
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    timer.tick();
    int overlaps = 0;
    for (const auto& arr : pairs) {
        int left  = arr[0] - arr[2];
        int right = arr[3] - arr[1];

        if ((right >= 0) && (left >= 0))
            overlaps++;
        else if ((right <= 0) && (left <= 0))
            overlaps++;
    }
    
    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << overlaps << endl;

    timer.tick();
    overlaps = 0;
    for (const auto& arr : pairs) {
        int left  = arr[3] - arr[0];
        int right = arr[1] - arr[2];

        if ((left >= 0) && (right >= 0))
            overlaps++;
    }

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << overlaps << endl;

    return 0;
}   