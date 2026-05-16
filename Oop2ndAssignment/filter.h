#ifndef FILTER_H
#define FILTER_H

#include <string>
using namespace std;

class filter {
public:
    virtual bool matchesFilter(string key) const = 0;
    virtual ~filter() {}
};

#endif