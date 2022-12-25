#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>
#include <unordered_map>
#include <unordered_set>

#include "clock.h"

using namespace std;

Timer timer;

// struct Cube {
//     Cube () { };
//     array<int, 3> loc;
//     int side = 6;
// };

// struct Cube {
//     Cube () { };
//     Cube (int a, int b, int c) : loc{a, b, c} { };
//     array<int, 3> loc;
//     unordered_set<array<int,3>,array_hash> checked;
// };


// array<int,3> operator=(const array<int,3>& lhs, const array<int,3>& rhs) {
//     return array<int,3>{lhs[0]+rhs[0], lhs[1]+rhs[1], lhs[2]+rhs[2]};
// }

// bool operator==(const Cube& lhs, const Cube& rhs) {
//     return lhs.loc == rhs.loc;
// };

struct array_hash {
    size_t operator() (const array<int, 3>& arr) const {
        return hash<int>{}(arr[0]) ^ hash<int>{}(arr[1]) ^ hash<int>{}(arr[2]);
    }
};

// struct cube_hash {
//     size_t operator() (const Cube& cube) const {
//         return hash<int>{}(cube.loc[0]) ^ hash<int>{}(cube.loc[1]) ^ hash<int>{}(cube.loc[2]);
//     }
// };


int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    unordered_map<array<int,3>, pair<int, unordered_set<array<int,3>, array_hash>>, array_hash> cubeSet; 
    ifstream ifile("input.txt", std::ios::in);
    regex rx("\\d+");
    while (getline(ifile, line, '\n')) {
        
        // Cube cube = Cube();
        sregex_iterator it(line.begin(), line.end(), rx);
        int i = 0;
        array<int, 3> cube = {0};
        // Cube cube = Cube();
        while (it != sregex_iterator()) {
            smatch match = *it++;
            cube[i++] = stoi(match.str()); 
            // cube.loc[i++] = stoi(match.str());
        }
        cubeSet.insert(pair(cube,pair(6, unordered_set<array<int,3>, array_hash>())));
    }
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Size: " << cubeSet.size() << endl;

    constexpr const array<array<int, 3>,6> directions =
        {array<int,3>{1, 0, 0}, array<int,3>{-1, 0, 0}, 
         array<int,3>{0, 1, 0}, array<int,3>{0, -1, 0}, 
         array<int,3>{0, 0, 1}, array<int,3>{0, 0, -1}};

    timer.tick();
    for (auto& [cube, checked] : cubeSet) {
        // auto [x, y, z] = cube.first;        

        for (const auto& dir : directions) {
            array<int, 3> tmp{cube[0] + dir[0], cube[1] + dir[1], cube[2] + dir[2]};

            if (cubeSet.contains(tmp)) {
                if (!checked.second.contains(tmp)) {
                    checked.second.insert(tmp);
                    checked.first--;
                    cubeSet.at(tmp).first--;
                    cubeSet.at(tmp).second.insert(cube);
                }
                // cube.second--;
                // cubeSet.at(tmp)--;
            }
        }
    }
    // cout << cubes.size() << endl;
    int sumSides = 0;
    for (const auto& [_, checked] : cubeSet)
        sumSides += checked.first;


    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << sumSides << endl;

    // cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    // cout << "Part 2: " << overlaps << endl;

    return 0;
}   