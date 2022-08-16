#include "function_collection.h"

FunctionCollection::FunctionCollection(const Function &function) {
    functions.insert(std::make_pair(function.getName(), function));
}

FunctionCollection::FunctionCollection(const std::vector<Function> &functions) {
    for (const Function &func : functions)
        this->functions.insert(std::make_pair(func.getName(), func));
}

size_t FunctionCollection::size() const { return functions.size(); }

void FunctionCollection::addFunction(const Function &function) {
    functions.insert(std::make_pair(function.getName(), function));
}

void FunctionCollection::removeFunction(const std::string &name) { functions.erase(name); }

Function &FunctionCollection::getFunction(const std::string &name) {
    std::map<std::string, Function>::iterator iter{functions.find(name)};
    if (iter != functions.end())
        return iter->second;
    else
        throw std::runtime_error("Undefined function " + name + "!");
}
