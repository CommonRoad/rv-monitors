#include "monitors/collections/domain2.h"
#include <utility>

DomainStorage2::DomainStorage2(std::string name) : name(std::move(name)), constDomainStorage(true) {}

std::string DomainStorage2::getName() const { return name; }

DomainStorage2::DomainStorage2(std::string name, const std::vector<std::any> &values,
                               const std::function<std::string(std::any)> &idFunction)
    : name(std::move(name)), values(values), constDomainStorage(true), idFunction(idFunction) {}

std::vector<std::any>::iterator DomainStorage2::begin() { return values.begin(); }

std::vector<std::any>::iterator DomainStorage2::end() { return values.end(); }

void DomainStorage2::addValues(const std::vector<std::any> &vals, const std::function<std::string(std::any)> &func) {
    values = vals;
    idFunction = func;
}

std::vector<std::any> DomainStorage2::getValues() const { return values; }

size_t DomainStorage2::size() const { return values.size(); }

bool DomainStorage2::empty() const { return values.empty(); }

DomainStorage2::DomainStorage2(std::string name, const IFunction<std::vector<std::any>> &dynamicDomainStorageFunction,
                               const std::function<std::string(std::any)> &idFunction,
                               std::function<bool(size_t, std::any)> validityCondition)
    : name(std::move(name)), dynamicDomainStorageFunction(std::move(dynamicDomainStorageFunction)),
      constDomainStorage(false), validityCondition(std::move(validityCondition)), idFunction(idFunction) {}

std::vector<std::any>::iterator DomainStorage2::updateDynamicDomain(const std::vector<DomainValue> &args) {
    if (!constDomainStorage)
        values = dynamicDomainStorageFunction.getFunction(args)();
    return begin();
}

void DomainStorage2::reset() {
    if (!constDomainStorage)
        values.clear();
}
bool DomainStorage2::isConstDomain() const { return constDomainStorage; }

const std::function<bool(size_t, std::any)> &DomainStorage2::getValidityCondition() const {
    return validityCondition.value();
}

const std::function<std::string(std::any)> &DomainStorage2::getIdFunction() const { return idFunction; }
