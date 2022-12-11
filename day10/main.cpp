#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>

#include "clock.h"

using namespace std;

Timer timer;

inline int getSignalStrength(int cpuCycle, int xRegister) {
    int tmp = 0;
    if (((cpuCycle + 20) % 40) == 0) {
        tmp = cpuCycle*xRegister;   
    }
    return tmp;
} 

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<pair<int, int>> instructions;
    ifstream ifile("input.txt", std::ios::in);
    // regex rx("\\d+");
    while (getline(ifile, line, '\n')) {
        
        pair<int, int> tmp;

        if (line.starts_with("addx"))
            tmp = {1, stoi(string(line.begin()+5, line.end()))};
        else
            tmp = {0, 0};

        instructions.push_back(tmp);
    }
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;
    
    int cpuCycle = 0;
    int xRegister = 1;
    int signalStrength = 0;
    for (const auto& [ins, increment] : instructions) {

        if (ins == 0) {
            cpuCycle++;
            signalStrength += getSignalStrength(cpuCycle, xRegister);
        } else {
            for (int i = 0; i < 2; i++) {
                cpuCycle++;
                signalStrength += getSignalStrength(cpuCycle, xRegister);
            }
            xRegister+=increment;
        }
    }

    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << signalStrength << endl;

    array<char,40*6> crt;
    fill(crt.begin(), crt.end(), ' ');

    cpuCycle = 0;
    xRegister = 1;
    signalStrength = 0;
    int offset = 0;
    for (const auto& [ins, increment] : instructions) {

        if (ins == 0) {
            if (((xRegister-1) <= cpuCycle) && ((xRegister + 1) >= cpuCycle)) {
                crt[cpuCycle + offset] = '#';
            }
            cpuCycle++;
            if (cpuCycle > 39) {
                cpuCycle = 0;
                offset+=40;
            }
        } else {
            for (int i = 0; i < 2; i++) {
                if (((xRegister-1) <= cpuCycle) && ((xRegister + 1) >= cpuCycle)) {
                    crt[cpuCycle + offset] = '#';
                }
                cpuCycle++;
                if (cpuCycle > 39) {
                    cpuCycle = 0;
                    offset+=40;
                }
            }
            xRegister+=increment;
        }
    }

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    // cout << "Part 2: " << overlaps << endl;

    for (size_t i = 0; i < crt.size(); i++) {
        cout << crt[i];
        if ((i+1) % 40 == 0)
            cout << '\n';
    }

    return 0;
}   