#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>

#include "clock.h"

using namespace std;

struct File {
    File(string& n, int s) : name(n), size(s) { }
    string name;
    int    size;
};

struct Directory {
    Directory(Directory* dir, string n) : parent(dir), name(n) { };
    Directory* parent;
    string name;
    unordered_map<string, Directory*> children;
    vector<File> files;
};

Timer timer;

int traverseTree(Directory* node, int& count) {
    int sum = 0;

    for (const auto& file : node->files)
        sum += file.size;

    for (const auto& child : node->children)
        sum += traverseTree(child.second, count);

    if (sum <= 100000 && node->parent != nullptr)
        count += sum;

    return sum;
}

int traverseTreeMin(Directory* node, pair<string, int>& minSize, const int dirTotal) {
    int sum = 0;

    for (const auto& file : node->files)
        sum += file.size;

    for (const auto& child : node->children)
        sum += traverseTreeMin(child.second, minSize, dirTotal);

    if (((dirTotal - sum) <= 40000000) && (sum < minSize.second)) {
        minSize.first = node->name;
        minSize.second = sum;
    }

    return sum;
}

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    ifstream ifile("input.txt", std::ios::in);

    Directory root(nullptr, "");
    root.children["/"] = new Directory(&root, "/");
    Directory* dirptr = &root;

    while (getline(ifile, line)) {
        
        if (line.starts_with("$ ls")) {
            getline(ifile, line);
        }
        else if (line.starts_with("$ cd")) {
            string dirName(line.begin()+5,line.end());
            if (dirName == "..")
                dirptr = dirptr->parent;
            else
                dirptr = dirptr->children.at(dirName);
            
            continue;
        }

        if (line.starts_with("dir")) {
            string child(line.begin()+4, line.end());
            dirptr->children[child] = new Directory(dirptr, child); 
        } else {
            int space = line.find(" ");
            int fileSize = stoi(string(line.begin(), line.begin()+space));
            string fileName(line.begin()+space+1, line.end()); 
            dirptr->files.push_back(File(fileName, fileSize));
        }
    }
    ifile.close();
    
    int total = 0, dirTotal = 0;
    pair<string, int> minSize("/", INT_MAX);
    dirptr = &root;
    dirTotal = traverseTree(dirptr, total);
    traverseTreeMin(dirptr, minSize, dirTotal);

    cout << total << endl;
    cout << dirTotal << endl;
    cout << "dir: " << minSize.first << endl;
    cout << "size: " << minSize.second << endl;

    return 0;
}   