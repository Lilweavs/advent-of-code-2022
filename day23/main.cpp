#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#include "clock.h"

using namespace std;

Timer timer;

template<class T>
T sgn(T a) {
    return (a < 0) ? -1 : 1;
}

struct Elf {
    Elf(int x, int y) : row(x), col(y) { }
    int row;
    int col;
};

bool operator==(const Elf& lhs, const Elf& rhs) {
    return lhs.row == rhs.row && lhs.col == rhs.col;
};

struct elf_hash {
    size_t operator() (const Elf& elf) const {
        return hash<int>{}(elf.row) ^ hash<int>{}(elf.col);
    }
};

bool checkAdjacent(Elf& elf, unordered_set<Elf, elf_hash>& currentPositions) {
    static array<pair<int,int>,8> directions = {pair{-1,-1}, {-1,0}, {-1,1}, {1,-1}, {1,0}, {1,1}, {0,1}, {0,-1}};
    bool adjacent = false;

    for (const auto& dir : directions) {
        Elf tmp(elf.row + dir.first, elf.col + dir.second);
        if (currentPositions.contains(tmp)) {
            adjacent = true;
            break;
        }
    }

    return adjacent;
};

bool stepElves(vector<Elf>& elves, string& dirOrder, unordered_set<Elf, elf_hash>& currentPositions) {

    unordered_map<Elf, int, elf_hash> suggestions;

    for (size_t i = 0; i < elves.size(); i++) {
        auto [row, col] = elves[i];

        if (!checkAdjacent(elves[i], currentPositions)) { continue; }

        for (const auto dir : dirOrder) {

            Elf cElf(row, col);
            Elf lElf(row, col);
            Elf rElf(row, col);

            switch (dir)
            {
            case 'N':
                cElf.row--;
                lElf.row--; lElf.col--;
                rElf.row--; rElf.col++;
                break;
            case 'S':
                cElf.row++;
                lElf.row++; lElf.col--;
                rElf.row++; rElf.col++;
                break;
            case 'W':
                cElf.col--;
                lElf.col--; lElf.row++;
                rElf.col--; rElf.row--;
                break;
            case 'E':
                cElf.col++;
                lElf.col++; lElf.row++;
                rElf.col++; rElf.row--;
                break;
            }

            if (!currentPositions.contains(lElf) && !currentPositions.contains(cElf) && !currentPositions.contains(rElf)) {
                if (suggestions.contains(cElf))
                    suggestions.at(cElf) = -1;
                else
                    suggestions[cElf] = i;
                break;
            }
        }
    }
    dirOrder += dirOrder.front();;
    dirOrder.erase(0,1);

    for (auto& [suggestion, index] : suggestions) {
        if (index != -1)
            elves[index] = suggestion;
    }

    return suggestions.empty();
}

void printElves(vector<Elf>& elves) {

    pair<int,int> minBound(INT_MAX, INT_MAX);
    pair<int,int> maxBound(INT_MIN, INT_MIN);
    // first  = row
    // second = col
    for (const auto& elf : elves) {
        minBound.first  = (minBound.first  > elf.row) ? elf.row : minBound.first;
        minBound.second = (minBound.second > elf.col) ? elf.col : minBound.second;
        maxBound.first  = (maxBound.first  < elf.row) ? elf.row : maxBound.first;
        maxBound.second = (maxBound.second < elf.col) ? elf.col : maxBound.second;
    }

    int rowOffset = (minBound.first  < 0) ? -minBound.first  : 0;
    int colOffset = (minBound.second < 0) ? -minBound.second : 0;

    vector<string> output;
    for (int row = 0; row <= (maxBound.first + rowOffset); row++)
        output.push_back(string(maxBound.second + colOffset+1, '.'));

    for (const auto& elf : elves)
        output[elf.row+rowOffset][elf.col+colOffset] = '#';

    for (const auto& o : output)
        cout << o << endl;
}

int main(int argc, char* argv[]) {

    timer.tick();
    string line;

    vector<Elf> elves;
    string dirOrder = "NSWE";
    unordered_set<Elf, elf_hash> currentPositions;
    unordered_map<Elf, int, elf_hash> suggestions;

    ifstream ifile("input.txt", std::ios::in);
    size_t row = 0;
    while (getline(ifile, line, '\n')) {
        for (size_t col = 0; col < line.length(); col++) {
            if (line[col] == '#') {
                elves.push_back(Elf(row, col));
                currentPositions.insert(Elf(row,col));
            }
        }
        row++;
    }

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    // printElves(elves);
    timer.tick();
    int round = 0;
    while (!stepElves(elves, dirOrder, currentPositions)) {
        currentPositions.clear();
        for (const auto& elf : elves) { currentPositions.insert(elf); }
        round++;

        if (round == 10) {
            pair<int,int> minBound(INT_MAX, INT_MAX);
            pair<int,int> maxBound(INT_MIN, INT_MIN);
            // first  = row
            // second = col
            for (const auto& elf : elves) {
                minBound.first  = (minBound.first  > elf.row) ? elf.row : minBound.first;
                minBound.second = (minBound.second > elf.col) ? elf.col : minBound.second;
                maxBound.first  = (maxBound.first  < elf.row) ? elf.row : maxBound.first;
                maxBound.second = (maxBound.second < elf.col) ? elf.col : maxBound.second;
            }

            int width  = abs(maxBound.second - minBound.second) + 1;
            int height = abs(maxBound.first - minBound.first) + 1;

            cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
            cout << "Part 1: " << (width * height) - elves.size() << endl;
            timer.tick();
        }
    }

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << round+1 << endl;

    return 0;
}