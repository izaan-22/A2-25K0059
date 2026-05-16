#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
using namespace std;

class display {
public:
    virtual void displayDetails() const = 0;
    virtual string getType() const = 0;
    virtual ~display() {}
};

#endif