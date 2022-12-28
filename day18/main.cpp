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

struct array_hash {
    size_t operator() (const array<int, 3>& arr) const {
        return hash<int>{}(arr[0]) ^ hash<int>{}(arr[1]) ^ hash<int>{}(arr[2]);
    }
};

typedef unordered_map<array<int,3>, pair<int, unordered_set<array<int,3>, array_hash>>, array_hash> CubeMap;

bool operator<(const array<int,3>& lhs, const array<int,3>& rhs) {
    return lhs[0] < rhs[0] || lhs[1] < rhs[1] || lhs[2] < rhs[2];
}

bool operator>(const array<int,3>& lhs, const array<int,3>& rhs) {
    return lhs[0] > rhs[0] || lhs[1] > rhs[1] || lhs[2] > rhs[2];
}

constexpr const array<array<int, 3>,6> directions =
    {array<int,3>{1, 0, 0}, array<int,3>{-1, 0, 0}, 
     array<int,3>{0, 1, 0}, array<int,3>{0, -1, 0}, 
     array<int,3>{0, 0, 1}, array<int,3>{0, 0, -1}};


bool airPocket(array<int,3> start, const CubeMap& cubeMap, const array<int,3>& negLimit, const array<int,3>& posLimit, unordered_set<array<int,3>, array_hash>& droplet) {

    bool airPocket = false;

    vector<array<int,3>> notVisited = {start};
    
    while (!notVisited.empty()) {

        array<int,3> cube = notVisited.back();
        droplet.insert(cube);
        
        if (cube < negLimit || cube > posLimit) { break; }

        notVisited.pop_back();



        for (const auto& dir : directions) {
            array<int, 3> tmp{cube[0] + dir[0], cube[1] + dir[1], cube[2] + dir[2]};
            if (!cubeMap.contains(tmp)) {
                if (!droplet.contains(tmp)) {
                    notVisited.push_back(tmp);
                }
            }            
        }
    }

    if (notVisited.empty()) { airPocket = true; }
    return airPocket;
}


int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    CubeMap cubeSet; 
    unordered_set<array<int,3>, array_hash> candidates;
    vector<array<int,3>> places;
    ifstream ifile("input.txt", std::ios::in);
    regex rx("\\d+");
    while (getline(ifile, line, '\n')) {
        
        sregex_iterator it(line.begin(), line.end(), rx);
        int i = 0;
        array<int, 3> cube = {0};
        while (it != sregex_iterator()) {
            smatch match = *it++;
            cube[i++] = stoi(match.str()); 
        }
        cubeSet.insert(pair(cube,pair(6, unordered_set<array<int,3>, array_hash>())));
    }
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    // Get upper and lower limits of droplet    
    array<int, 3> mostNegative = {INT_MAX};
    array<int, 3> mostPositive = {INT_MIN};
    for (const auto& [cube, _] : cubeSet) {
        mostNegative[0] = (mostNegative[0] > cube[0]) ? cube[0] : mostNegative[0];
        mostNegative[1] = (mostNegative[1] > cube[1]) ? cube[1] : mostNegative[1];
        mostNegative[2] = (mostNegative[2] > cube[2]) ? cube[2] : mostNegative[2];
        mostPositive[0] = (mostPositive[0] < cube[0]) ? cube[0] : mostPositive[0];
        mostPositive[1] = (mostPositive[1] < cube[1]) ? cube[1] : mostPositive[1];
        mostPositive[2] = (mostPositive[2] < cube[2]) ? cube[2] : mostPositive[2];
    }

    timer.tick();
    for (auto& [cube, checked] : cubeSet) {
        // auto [x, y, z] = cube.first;        

        for (const auto& dir : directions) {
            array<int, 3> tmp{cube[0] + dir[0], cube[1] + dir[1], cube[2] + dir[2]};

            if (cubeSet.contains(tmp) && !checked.second.contains(tmp)) {
                checked.second.insert(tmp);
                checked.first--;
                cubeSet.at(tmp).first--;
                cubeSet.at(tmp).second.insert(cube);
            }
        }
    }

    int sumSides = 0;
    for (const auto& [_, checked] : cubeSet)
        sumSides += checked.first;

    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << sumSides << endl;

    timer.tick();
    // Find vector of potential air pockets.
    for (const auto& [cube, _] : cubeSet) {
        for (const auto& dir : directions) {
            array<int, 3> tmp{cube[0] + dir[0], cube[1] + dir[1], cube[2] + dir[2]};
            if (!cubeSet.contains(tmp) && !candidates.contains(tmp)) {
                candidates.insert(tmp);
                places.push_back(tmp);
            }
        }
    }

    for (const auto& v : places) {

        if (!candidates.contains(v)) { continue; }      

        unordered_set<array<int,3>, array_hash> droplet;

        if (airPocket(v, cubeSet, mostNegative, mostPositive, droplet)) {
            for (const auto& air : droplet) {
                for (const auto& dir : directions) {
                    array<int, 3> tmp{air[0] + dir[0], air[1] + dir[1], air[2] + dir[2]};

                    if (cubeSet.contains(tmp))
                        cubeSet.at(tmp).first--;
                }
                candidates.erase(air);
            }
        } else {
            // Remove all connecting cubes 
            for (const auto& air : droplet) {
                candidates.erase(air);
            }
        };
    }

    sumSides = 0;
    for (const auto& [_, checked] : cubeSet)
        sumSides += checked.first;

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << sumSides - 6*candidates.size() << endl;

    return 0;
}   