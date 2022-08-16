#include "domain.h"

#include <utility>

Domain::Domain(std::string name, std::vector<Value> values) : name(std::move(name)), values(std::move(values)) {}

Domain::Domain(std::string name, const std::vector<std::string> &values) : name(std::move(name)) {
    for (const std::string &val : values)
        this->values.emplace_back(val);
}

Domain Domain::operator+(const Domain &other) {
    Domain domain{this->name + other.name, this->values};
    domain.addValues(other.values);
    return domain;
}

size_t Domain::size() const { return values.size(); }

std::vector<Value>::iterator Domain::begin() { return values.begin(); }

std::vector<Value>::iterator Domain::end() { return values.end(); }

void Domain::addValue(const std::string &val) { values.emplace_back(val); }

void Domain::addValue(const Value &val) { values.push_back(val); }

void Domain::addValues(const std::vector<std::string> &vals) {
    for (const std::string &val : vals)
        values.emplace_back(val);
}
void Domain::addValues(const std::vector<Value> &vals) {
    for (const Value &val : vals)
        values.push_back(val);
}
float Domain::calcWeightAverage() const {
    float summedWeights = 0.;
    for (const Value &val : values)
        summedWeights += val.getWeight();
    return summedWeights / static_cast<float>(size());
}

std::string Domain::getName() const { return name; }
