#include "monitors/collections/constant_collection.h"

ConstantCollection::ConstantCollection(const std::vector<ConstantValue> &constants) {
    for (const auto &constant : constants)
        this->constants.insert(std::make_pair(constant.getName(), constant));
}

size_t ConstantCollection::size() const { return constants.size(); }

void ConstantCollection::addConstant(const ConstantValue &constant) {
    this->constants.insert(std::make_pair(constant.getName(), constant));
}

ConstantValue &ConstantCollection::getConstant(const std::string &name) { return constants.at(name); }

bool ConstantCollection::contains(const std::string &name) { return constants.find(name) != constants.end(); }
