#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <queue>

#include "clock.h"

using namespace std;

Timer timer;

struct Point {
    int x = 0;
    int y = 0;
};

Point operator+(const Point& lhs, const Point& rhs) {
    return Point(lhs.x + rhs.x, lhs.y + rhs.y);
}

bool operator==(const Point& lhs, const Point& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

struct struct_hash
{
    size_t operator() (const Point& p) const
    {
        return hash<int>{}(p.x) ^ hash<int>{}(p.y);
    }
};


int findPath(vector<string>& grid, Point startPoint, Point endPoint) {
    static array<Point,4> directions = {Point(1,0), Point(0,1), Point(0,-1), Point(-1,0)};

    queue<pair<Point, Point>> notVisited;
    unordered_map<Point, Point, struct_hash> visited;
    
    int rowMax = grid[0].length() - 1;
    int colMax = grid.size() - 1;

    notVisited.push({startPoint, startPoint});
    while (notVisited.size() != 0) {

        auto [currentPoint, parentPoint] = notVisited.front();
        notVisited.pop();

        if (visited.contains(currentPoint)) { continue; }
        
        visited.insert({currentPoint, parentPoint});

        if (currentPoint == endPoint) {
            int cnt = 0;
            while (currentPoint != startPoint) {
                currentPoint = visited[currentPoint];
                cnt++;
            }
            return cnt;
        }

        for (const auto& dir : directions) {
            Point tmp = currentPoint + dir;
            if (!(tmp.x < 0 || tmp.x > rowMax || tmp.y < 0 || tmp.y > colMax)) {
                if (!visited.contains(tmp)) {
                    char height = grid[currentPoint.y][currentPoint.x];
                    if ((height+1) >= grid[tmp.y][tmp.x])
                        notVisited.push({tmp, currentPoint});
                }
            }
        }
    }

    return INT_MAX;
}

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<array<int,4>> pairs;
    ifstream ifile("input.txt", std::ios::in);
    
    vector<string> grid;

    while (getline(ifile, line, '\n')) {
        grid.push_back(line);
    }
    ifile.close();

    Point startPoint, endPoint;
    for (size_t i = 0; i < grid.size(); i++) {
        if (grid[i].find('S') != string::npos) {
            startPoint.x = grid[i].find('S');
            startPoint.y = i;
            grid[startPoint.y][startPoint.x] = 'a';
        }
        if (grid[i].find('E') != string::npos) {
            endPoint.x = grid[i].find('E');
            endPoint.y = i;
            grid[endPoint.y][endPoint.x] = 'z';
        }
    }

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    timer.tick();
    int minPathLength = findPath(grid, startPoint, endPoint);

    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << minPathLength << endl;

    minPathLength = INT_MAX;
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].length(); j++) {
            if (grid[i][j] == 'a') {
                int tmp = findPath(grid, Point(j,i), endPoint);
                minPathLength = (minPathLength > tmp) ? tmp : minPathLength;
            }
        }
    }

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << minPathLength << endl;

    return 0;
}   