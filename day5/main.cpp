#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>

#include "clock.h"

using namespace std;

Timer timer;

string getTopCargo(const array<string,9>& cargo) {
    string ans;
    for (const auto& str : cargo)
        ans += str.back();
    return ans;
}

string part1(const vector<array<int,3>>& instructions, array<string,9> cargo) {

    for (const auto& ins : instructions) {
        const auto [amt, src, dest] = ins;

        cargo[dest].append(cargo[src].rbegin(), cargo[src].rbegin()+amt);
        cargo[src].erase(cargo[src].length()-amt, amt);
    }

    return getTopCargo(cargo);
}

string part2(const vector<array<int,3>>& instructions, array<string,9> cargo) {

    for (const auto& ins : instructions) {
        const auto [amt, src, dest] = ins;

        cargo[dest].append(cargo[src].end()-amt, cargo[src].end());
        cargo[src].erase(cargo[src].length()-amt, amt);
    }

    return getTopCargo(cargo);
}

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
                cargo[i] += c;
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
        arr[1] -= 1;
        arr[2] -= 1;
        instructions.push_back(arr);
    }
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    cout << "Part 1: " << part1(instructions, cargo) << endl;
    cout << "Part 2: " << part2(instructions, cargo) << endl;

    return 0;
}