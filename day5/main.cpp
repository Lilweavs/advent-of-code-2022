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
    vector<array<int,3>> instructions;
    array<string,9> cargo;
    
    ifstream ifile("input.txt", std::ios::in);
    while (getline(ifile, line, '\n')) {
        if (line.empty()) { break; }
        for (size_t i = 0; i < cargo.size(); i++) {
            char c = line[1 + (4*i)];
            if (c == '1') { break; }
            if (c != ' ')
                cargo[i] += line[1 + (4*i)]; 
        }
    }
    for (auto& str : cargo)
        reverse(str.begin(), str.end());

    regex rx("\\d+");
    while (getline(ifile, line, '\n')) {
        array<int, 3> arr;
        sregex_iterator it(line.begin(), line.end(), rx);
        int i = 0;
        while (it != sregex_iterator()) {
            smatch match = *it++;
            arr[i++] = stoi(match.str());   
        }
        instructions.push_back(arr);
    }
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    /* Part 1 */
    for (const auto& ins : instructions) {
        int amt = ins[0];
        int src = ins[1] - 1;
        int dest = ins[2] - 1;

        int length = cargo[src].length();
        // for (int i = length; i > (length - amt); i--) {
        //     cargo[dest] += cargo[src][i-1];
        //     cargo[src].pop_back();
        // }
        // for (int i = length - amt; i < length; i++) {
        string sub = cargo[src].substr(length - amt, amt);
        cargo[dest] += sub;
        cargo[src].erase(length - amt, amt);
        // }
    }
    
    
    string ans;
    for (const auto& str : cargo) {
        ans += str.back();
    }

    cout << ans;
    /* Part 2 */

    
    return 0;
}   