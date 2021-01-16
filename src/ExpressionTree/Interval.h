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

    void reduceInterval();

    [[nodiscard]] int getStart() const;

    void setStart(int start);

    [[nodiscard]] int getAnEnd() const;

    void setAnEnd(int anEnd);

    [[nodiscard]] bool isFinite() const;

    void setFinite(bool finite);

    [[nodiscard]] bool containsZero() const;

private:
    int start;
    int end;
    bool finite;
};


#endif //RV_MONITORS_INTERVAL_H
