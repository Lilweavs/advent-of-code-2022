#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>

#include "clock.h"

using namespace std;

#define MARK_LENGTH 4
#define MSG_LENGTH  14

Timer timer;

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    ifstream ifile("input.txt", std::ios::in);
    getline(ifile, line, '\n');

    unordered_set<char> packetStart;

    int index = 0;
    for (size_t i = 0; i < line.length() - MARK_LENGTH; i++) {
        packetStart.insert(line.begin()+i, line.begin() + i + MARK_LENGTH);

        if (packetStart.size() == MARK_LENGTH) {
            index = i + MARK_LENGTH;
            break;
        }
        packetStart.clear();
    }

    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << index << endl;

    timer.tick();
    index = 0;
    for (size_t i = 0; i < line.length() - MSG_LENGTH; i++) {
        for (size_t j = 0; j < i + MSG_LENGTH; j++) {
            packetStart.insert(line[j]);
            if (packetStart.size() != (j + 1)) { break; }
        }

        if (packetStart.size() == MSG_LENGTH) {
            index = i + MSG_LENGTH;
            break;
        }
        packetStart.clear();
    }

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << index << endl;

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    return 0;
}   