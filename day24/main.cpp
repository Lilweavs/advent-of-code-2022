#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_set>
#include <algorithm>

#include "clock.h"

using namespace std;

Timer timer;

struct Blizzard {
    Blizzard(char c_, int x, int y) : c(c_), row(x), col(y) { };
    char c = '\0';
    int row  = 0;
    int col  = 0;
};

void printGrid(vector<string>& grid) {
    for (const auto& str : grid)
        cout << str << endl;
}

struct pair_hash {
    size_t operator() (const pair<int,int>& p) const {
        return hash<int>{}(p.first) ^ hash<int>{}(p.second);
    }
};

void stepBlizzard(vector<string>& grid, vector<Blizzard>& blizzards, vector<vector<int>>& traffic) {
    for (auto& blizzard : blizzards) {
        auto [c, row, col] = blizzard;
    
        traffic[row][col]--;
        if (traffic[row][col] == 0) { grid[row][col] = '.'; }
        
        if (c == '>') {
            if (grid[row][col+1] == '#')
                blizzard.col = 1;
            else
                blizzard.col++;
        } else if (c == '<') {
            if (grid[row][col-1] == '#')
                blizzard.col = grid[row].length() - 2;
            else
                blizzard.col--;
        } else if (c == '^') {
            if (grid[row-1][col] == '#')
                blizzard.row = grid.size() - 2;
            else
                blizzard.row--;
        } else {
            if (grid[row+1][col] == '#')
                blizzard.row = 1;
            else
                blizzard.row++;
        }
        grid[blizzard.row][blizzard.col] = c;
        traffic[blizzard.row][blizzard.col]++;
    }
}

void resetGrid(vector<string> grid) {
    for (auto& str : grid)
        replace(str.begin(), str.end(), 'E', '.');
}

int findMinimumMinutes(vector<string>& grid, vector<Blizzard>& blizzards, vector<vector<int>>& traffic, pair<int,int> start, pair<int,int> end) {
    static array<pair<int,int>,5> directions = {pair{1,0},{-1,0},{0,1},{0,-1},{0,0}};

    unordered_set<pair<int,int>, pair_hash> safeLocations({start});

    int minute = 0;
    bool found = false;
    while (!found) {
        vector<pair<int,int>> tmpSafeLocations;
        
        stepBlizzard(grid, blizzards, traffic);

        for (auto& elf : safeLocations) {
            
            for (const auto& dir : directions) {
                pair<int,int> tmp = {elf.first + dir.first, elf.second + dir.second};
            
                if (tmp.first < 0 || tmp.second < 1 || tmp.first == (int)grid.size() || grid[tmp.first][tmp.second] == '#') { continue; }

                if (traffic[tmp.first][tmp.second] == 0) { grid[tmp.first][tmp.second] = '.'; }

                if (tmp.first!= (int)grid.size() && grid[tmp.first][tmp.second] == '.') {
                    grid[tmp.first][tmp.second] = 'E';
                    tmpSafeLocations.push_back(tmp);
                }
                if (tmp.first == end.first && tmp.second == end.second) { found = true; break; }
            }            
        }

        safeLocations.clear();
        for (const auto& v : tmpSafeLocations) {
            safeLocations.insert(v);
        }
        minute++;
        // cout << "Minute: " << ++minute << " Size: " << safeLocations.size() << endl;
        // printGrid(grid);
    }
    return minute;
}


int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<string> grid;
    vector<Blizzard> blizzards;
    vector<vector<int>> traffic;

    ifstream ifile("input.txt", std::ios::in);
    size_t row = 0;
    while (getline(ifile, line, '\n')) {
        traffic.push_back(vector<int>());
        grid.push_back(line);
        for (size_t col = 0; col < line.length(); col++) {
            char c = line[col];
            traffic[row].push_back(0);    
            if (c == '>' || c == '<' || c == '^' || c == 'v') {
                blizzards.push_back(Blizzard(c, row, col));
                traffic[row][col]++;
            }
        }
        row++;
    }

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;
    // printGrid(grid);
    
    timer.tick();
    pair<int,int> start = {0,1};
    pair<int,int> end = {grid.size()-1, grid[0].length()-2};

    int minute = findMinimumMinutes(grid, blizzards, traffic, start, end);

    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << minute << endl;

    timer.tick();
    resetGrid(grid);
    minute += findMinimumMinutes(grid, blizzards, traffic, end, start);
    resetGrid(grid);
    minute += findMinimumMinutes(grid, blizzards, traffic, start, end);

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << minute << endl;

    return 0;
}





