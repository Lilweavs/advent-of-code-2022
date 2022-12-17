#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>

#include "clock.h"

using namespace std;

Timer timer;

struct Coords {
    int x = 0;
    int y = 0;
};

Coords operator-(const Coords &lhs, const Coords &rhs) {
    return Coords{lhs.x - rhs.x, lhs.y - rhs.y};
}

int simulateSand(vector<string>& grid, Coords& point, int bottom) {
    int col = 500;
    for (size_t row = 0; row < grid.size(); row++) {
        if (row == static_cast<size_t>(bottom)) {
            point.x = col;
            point.y = row-1;
            return 1;
        }
        char c = grid[row][col]; 
        if (c == 'O' || c == '#') {
            if (grid[row][col-1] == '.') {
                col--;
                row--;
            } else if (grid[row][col+1] == '.') {
                col++;
                row--;
            } else {
                point.x = col;
                point.y = row-1;
                return 1;
            }
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    array<string,9> cargo;
    regex rx("(\\d+),(\\d+)");

    vector<string> grid;
    vector<vector<Coords>> measurements;

    for (int i = 0; i < 200; i++) {
        string tmp;
        tmp.insert(0,1000,'.');
        grid.emplace_back(tmp);
    }

    int maxy = 0;
    ifstream ifile("input.txt", std::ios::in);
    while (getline(ifile, line, '\n')) {
        sregex_iterator it(line.begin(), line.end(), rx);

        vector<Coords> scan;
        while (it != sregex_iterator()) {
            smatch match = *it++;

            int tmp = stoi(match[2].str());
            maxy = (maxy < tmp) ? tmp : maxy;

            scan.push_back(Coords{stoi(match[1].str()),tmp});
        }
        measurements.push_back(scan);
    }
    maxy += 2;
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    for (const auto& scan : measurements) {
        for (size_t i = 1; i < scan.size(); i++) {
            Coords diff = scan[i] - scan[i-1];
            const auto& [x0, y0] = scan[i-1];

            if (diff.y == 0) {
                if (diff.x > 0) {
                    for (int j = x0; j <= x0 + diff.x; j++) {
                        grid[y0][j] = '#';
                    }
                } else {
                    for (int j = x0; j >= x0 + diff.x; j--) {
                        grid[y0][j] = '#';
                    }
                }
            } else {
                if (diff.y > 0) {
                    for (int j = y0; j <= y0 + diff.y; j++) {
                        grid[j][x0] = '#';
                    }
                } else {
                    for (int j = y0; j >= y0 + diff.y; j--) {
                        grid[j][x0] = '#';
                    }
                }
            }
        }
    }

    timer.tick();
    Coords tmp;
    int cnt = 0;
    while (simulateSand(grid, tmp, maxy) != -1) {
        if (tmp.y == maxy-1) { break; }
        grid[tmp.y][tmp.x] = 'O';
        cnt++;
    }
    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << cnt << endl;
    
    timer.tick();
    while (simulateSand(grid, tmp, maxy) != -1) {
        grid[tmp.y][tmp.x] = 'O';
        cnt++;
        if (tmp.y == 0) { break; }
    }

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << cnt << endl;

    // for (size_t row = 0; row < 14; row++) {
    //     for (size_t col = 480; col < 520; col++) {
    //         cout << grid[row][col];
    //     }
    //     cout << endl;;
    // }

    // cout << "Part 2: " << part2(instructions, cargo) << endl;

    return 0;
}