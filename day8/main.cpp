#include <iostream>
#include <fstream>
#include <vector>
#include <array>

#include "clock.h"

using namespace std;

Timer timer;

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<vector<int>> grid;
    ifstream ifile("input.txt", std::ios::in);
    while (getline(ifile, line, '\n')) {
        vector<int> tmp;
        for (auto c : line)
            tmp.push_back(c - 48);
        grid.push_back(tmp);
    }
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    timer.tick();
    int rowSize = grid.size();
    int colSize = grid[0].size();
    int visibleTrees = 2*(rowSize + colSize) - 4;
    for (size_t i = 1; i < static_cast<size_t>(rowSize-1); i++) {
        for (size_t j = 1; j < static_cast<size_t>(colSize-1); j++) {
            int tmp = grid[i][j];

            /* right */
            if (tmp > *max_element(grid[i].begin()+j+1, grid[i].end())) {
                visibleTrees++;
                continue;
            }
                
            /* left */    
            if (tmp > *max_element(grid[i].begin(), grid[i].begin()+j)) {
                visibleTrees++;
                continue;
            }
                
            /* down */
            int m = 0;
            for (int k = i+1; k < rowSize; k++) {
                if (m < grid[k][j])
                    m = grid[k][j];
            }

            if (tmp > m) {
                visibleTrees++;
                continue;
            }

            /* up */
            m = 0;
            for (int k = i-1; k >= 0; k--) {
                if (m < grid[k][j])
                    m = grid[k][j];
            }
            
            if (tmp > m)
                visibleTrees++;

        }
    }

    cout << "Time: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << visibleTrees << endl;

    timer.tick();
    int score = 1;
    pair<int,int> tree(1,1);
    for (size_t i = 1; i < static_cast<size_t>(rowSize-1); i++) {
        for (size_t j = 1; j < static_cast<size_t>(colSize-1); j++) {
            int tmp = grid[i][j];
            
            /* right */            
            int k = j + 1;
            while (k != colSize-1 && tmp > grid[i][k]) {
                k++;
            }
            int rightScore = k - j;

            /* left */    
            k = j - 1;
            while (k != 0 && tmp > grid[i][k]) {
                k--;
            }
            int leftScore = j - k;

            /* down */
            k = i + 1;
            while (k != rowSize-1 && tmp > grid[k][j]) {
                k++;
            }
            int downScore = k - i;

            /* up */
            k = i - 1;
            while (k != 0 && tmp > grid[k][j]) {
                k--;
            }
            int upScore = i - k;

            if (score < rightScore*leftScore*upScore*downScore) {
                score = rightScore*leftScore*upScore*downScore;
            }
        }
    }

    cout << "Time: " << timer.tock() / 1000.0 << "ms"  << endl;
    cout << "Part 2: " << score << endl;

    return 0;
}       