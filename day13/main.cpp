#include <iostream>
#include <fstream>
#include <vector>
#include <variant>
#include <memory>

#include "clock.h"

using namespace std;

Timer timer;

enum class Order {
    None,
    InOrder,
    NotInOrder
};

struct List {
    List(List* p) : parent(p) { };
    vector<variant<int, List*>> contents;
    List* parent;
};

Order compareTwoLists(List* pair1, List* pair2) {

    if (pair1->contents.empty() && !pair2->contents.empty()) { return Order::InOrder; }
    if (pair1->contents.empty() && pair1->contents.empty())  { return Order::None; }
    if (!pair1->contents.empty() && pair2->contents.empty()) { return Order::NotInOrder; }

    Order order = Order::None;

    size_t pair1Size = pair1->contents.size();
    size_t pair2Size = pair2->contents.size();

    size_t index = 0;
    for (size_t i = 0; i < pair1Size; i++) {
        
        if (order != Order::None) { return order; }
        if (i == pair2Size)   { return Order::NotInOrder; }
        
        variant<int, List*> left = pair1->contents[i];
        variant<int, List*> right = pair2->contents[i];
        
        if (holds_alternative<int>(left) && holds_alternative<int>(right)) {
            if (left > right)
                return Order::NotInOrder;
            else if (left < right)
                return Order::InOrder;
            
        } else if (holds_alternative<int>(left) && holds_alternative<List*>(right)) {
            /* Pass for now */
            List* rptr = get<List*>(right);

            List tmp(pair1);
            tmp.contents.push_back(left);

            order = compareTwoLists(&tmp, rptr);

        } else if (holds_alternative<List*>(left) && holds_alternative<int>(right)) {
            /* Pass for now */
            List* lptr = get<List*>(left);
            
            List tmp(pair2);
            tmp.contents.push_back(right);

            order = compareTwoLists(lptr, &tmp);

        } else {
            /* Pass for now */
            List* lptr = get<List*>(left);
            List* rptr = get<List*>(right);

            order = compareTwoLists(lptr, rptr);
        }
        index++;
    }

    if (order == Order::NotInOrder) { return Order::NotInOrder; }
    if (index < pair2Size) { return Order::InOrder; }
    return order;

}

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<pair<int, int>> instructions;
    ifstream ifile("input.txt", std::ios::in);

    int i = 1;
    int cnt = 0;
    while (getline(ifile, line, '\n')) {

        List pair1(nullptr);
        List pair2(nullptr);
        List* lptr = &pair1;
        // shared_ptr<List> ptr = make_shared<List>(pair1);

        for (size_t i = 1; i < line.length(); i++) {
            const char c = line[i];
            if (c == '[') {
                // if (i == 1) { continue; }
                variant<int, List*> tmp = new List(lptr);
                lptr->contents.push_back(tmp);
                lptr = get<List*>(tmp);
            } else if (c == ']') {
                lptr = lptr->parent;
            } else if (c == ',') {
                /* do nothing we don't care*/
            } else {
                /* Must be number */
                variant<int, List*> tmp = 0;
                if (((line[i+1] - '0') < 9) && (line[i+1] - '0' >= 0)) {
                    tmp = stoi(string({c,line[i+1]}));
                    i++;
                } else {
                    tmp = c - '0';
                }
                lptr->contents.push_back(tmp);
                
            }
        }

        getline(ifile, line);

        lptr = &pair2;

        for (size_t i = 1; i < line.length(); i++) {
            const char c = line[i];
            if (c == '[') {
                // if (i == 1) { continue; }
                variant<int, List*> tmp = new List(lptr);
                lptr->contents.push_back(tmp);
                lptr = get<List*>(tmp);
            } else if (c == ']') {
                lptr = lptr->parent;
            } else if (c == ',') {
                /* do nothing we don't care*/
            } else {
                /* Must be number */
                variant<int, List*> tmp = 0;
                if (((line[i+1] - '0') < 9) && (line[i+1] - '0' >= 0)) {
                    tmp = stoi(string({c,line[i+1]}));
                    i++;
                } else {
                    tmp = c - '0';
                }
                lptr->contents.push_back(tmp);
                
            }
        }

        getline(ifile, line);

        if (compareTwoLists(&pair1, &pair2) == Order::InOrder) {
            cnt += i;
        }
        i++;
    }
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << cnt << endl;


    vector<List> part2;

    List dividerOne = List(nullptr);
    List dividerTwo = List(nullptr);
    dividerOne.contents.push_back(new List(&dividerOne));
    dividerTwo.contents.push_back(new List(&dividerTwo));
    List* lptr = get<List*>(dividerOne.contents[0]);
    lptr->contents.push_back(variant<int, List*>(2));
    lptr = get<List*>(dividerTwo.contents[0]);
    lptr->contents.push_back(variant<int, List*>(6));

    part2.push_back(dividerOne);
    part2.push_back(dividerTwo);

    ifile.open("input.txt", std::ios::in);
    while (getline(ifile, line, '\n')) {

        if (line.empty()) { continue; }

        List pair1(nullptr);
        List* lptr = &pair1;

        for (size_t i = 1; i < line.length(); i++) {
            const char c = line[i];
            if (c == '[') {
                // if (i == 1) { continue; }
                variant<int, List*> tmp = new List(lptr);
                lptr->contents.push_back(tmp);
                lptr = get<List*>(tmp);
            } else if (c == ']') {
                lptr = lptr->parent;
            } else if (c == ',') {
                /* do nothing we don't care*/
            } else {
                /* Must be number */
                variant<int, List*> tmp = 0;
                if (((line[i+1] - '0') < 9) && (line[i+1] - '0' >= 0)) {
                    tmp = stoi(string({c,line[i+1]}));
                    i++;
                } else {
                    tmp = c - '0';
                }
                lptr->contents.push_back(tmp);
            }
        }

        part2.push_back(pair1);

    }
    ifile.close();

    sort(part2.begin(), part2.end(), [](List a, List b) { 
        return (compareTwoLists(&a,&b) == Order::InOrder) ? true : false; 
    });

    int a = 0;
    int b = 0;
    for (size_t i = 0; i < part2.size(); i++) {
        if (compareTwoLists(&dividerOne, &part2[i]) == Order::None) {
            a = i+1;
            break;
        }
    }

    for (size_t i = 0; i < part2.size(); i++) {
        if (compareTwoLists(&dividerTwo, &part2[i]) == Order::None) {
            b = i+1;
            break;
        }
    }

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: "<< a*b << endl;

    return 0;
}   