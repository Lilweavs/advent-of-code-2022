#include <iostream>
#include <fstream>

#include "clock.h"

using namespace std;

#define MARK_LENGTH 4
#define MSG_LENGTH  14

Timer timer;

int solve(string& input, size_t size) {
    
    for (size_t i = 0; i < input.length() - size; i++) {
        uint32_t index = 0;
        for (size_t j = i; j < i + size; j++) {
            uint32_t tmp = index;
            index |= (1U << (input[j] - 'a'));
            if (tmp == index) { 
                break; 
            } else if (j == i + size - 1) {
                return i + size;
            } else { /* Do nothing */}
        }
    }
    return -1; /* Shouldn't happen */
}

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    ifstream ifile("input.txt", std::ios::in);
    getline(ifile, line, '\n');

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    timer.tick();
    int index = solve(line, MARK_LENGTH);

    cout << "Part 1 opt Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1 opt: " << index << endl;

    timer.tick();
    index = solve(line, MSG_LENGTH);
    
    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2 : " << index << endl;

    return 0;
}   