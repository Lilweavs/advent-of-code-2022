#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>
#include <numeric>
#include <cstdlib>

#include "clock.h"

using namespace std;

Timer timer;

template<class T>
T sgn(T a) {
    return (a < 0) ? -1 : 1;
}

int64_t intPow(int64_t num, int pow) {
    int64_t tmp = 1;
    for (int i = 0; i < pow; i++) { tmp *= num; }
    return tmp;
}

int64_t PartialGeoSum(int64_t num, int pow) {
    return (intPow(num, pow+1) - 1) / (num - 1);
}

int64_t CalcSnafuBit(char c, size_t offset) {
    int64_t tmp = 0;
    switch (c)
    {
    case '1':
        tmp = 1*intPow(5, offset);
        break;
    case '2':
        tmp = 2*intPow(5, offset);
        break;
    case '-':
        tmp = -1*intPow(5, offset);
        break;
    case '=':
        tmp = -2*intPow(5, offset);
        break;
    default:
        break;
    }
    return tmp;
}

void ReverseSnafuBit(int64_t& sum, string& ans, int depth) {
    int i = 0;
    if (depth < 0) { return; }
    while (sgn(sum)*sum > 2*PartialGeoSum(5, i)) { i++; }
    if (depth == 0 && sum == 0) { ans+='0'; return; }
    if (i != depth) {
        ans += '0';
    } else {

        int64_t diff = sgn(sum)*sum - CalcSnafuBit('2', i); 
        if (sum > 0){
            if (-diff <= 2*PartialGeoSum(5,i-1)) {
                ans += '2';
                sum -= CalcSnafuBit('2', i);
            } else {
                ans += '1';
                sum -= CalcSnafuBit('1', i);
            }
        } else {
            if (-diff <= 2*PartialGeoSum(5,i-1)) {
                ans += '=';
                sum -= CalcSnafuBit('=', i);
            } else {
                ans += '-';
                sum -= CalcSnafuBit('-', i);
            }
        }
    }
    ReverseSnafuBit(sum, ans, depth - 1);
}

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<array<int,3>> instructions;

    vector<int64_t> snafuNumbers;

    ifstream ifile("input.txt", std::ios::in);
    while (getline(ifile, line, '\n')) {

        int64_t tmp = 0;
        for (size_t i = 0; i < line.length(); i++) {
            tmp += CalcSnafuBit(line[i], (line.length()-1) - i);
        }
        snafuNumbers.push_back(tmp);
    }

    int64_t sum = accumulate(snafuNumbers.begin(), snafuNumbers.end(), 0LL);

    // cout << "Part 1: " << sum << endl;

    timer.tick();
    string snafuNumber;
    int depth = 0;
    // Get most significant bit offset
    while (sgn(sum)*sum > 2*PartialGeoSum(5, ++depth)) { }

    ReverseSnafuBit(sum, snafuNumber, depth);

    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << snafuNumber << endl;

    return 0;
}





