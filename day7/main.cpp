#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>

#include "clock.h"

using namespace std;

struct File {
    File(string& n, int s) : name(n), size(s) { }
    string name;
    int    size;
};

struct Directory {
    Directory(Directory* dir) { parent = dir; }
    Directory* parent;
    unordered_map<string, Directory*> children;
    vector<File> files;
    ~Directory() {
        delete parent;
    }
};

Timer timer;

int traverseTree(Directory* parent, int& count) {
    int sum = 0;

    for (const auto& file : parent->files)
        sum += file.size;

    for (const auto& child : parent->children)
        sum += traverseTree(child.second, count);

    if (sum >= 100000)
        sum = 0;
    
    count += sum;
    return sum;
}

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    ifstream ifile("input.txt", std::ios::in);

    Directory root(nullptr);
    root.children["/"] = new Directory(&root);
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
            dirptr->children[child] = new Directory(dirptr); 
        } else {
            int space = line.find(" ");
            int fileSize = stoi(string(line.begin(), line.begin()+space));
            string fileName(line.begin()+space+1, line.end()); 
            dirptr->files.push_back(File(fileName, fileSize));
        }
    }
    ifile.close();
    
    int total = 0;
    dirptr = &root;
    traverseTree(dirptr, total);



    cout << total << endl;

    return 0;
}   