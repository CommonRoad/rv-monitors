//
// Created by Sebastian Maierhofer on 12.01.21.
//

#ifndef RV_MONITORS_INTERVAL_H
#define RV_MONITORS_INTERVAL_H

#include <string>

class Interval {
public:
    explicit Interval(int start=0, int end=10000, bool finite=false); // TODO introduce infinity
    [[nodiscard]] std::string to_string() const;
private:
    int start;
    int end;
    bool finite;
};


#endif //RV_MONITORS_INTERVAL_H
