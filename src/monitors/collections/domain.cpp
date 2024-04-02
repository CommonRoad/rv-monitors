#include "monitors/collections/domain.h"
#include <utility>

DomainStorage::DomainStorage(std::string name) : name(std::move(name)), constDomainStorage(true) {}

std::string DomainStorage::getName() const { return name; }

DomainStorage::DomainStorage(std::string name, const std::vector<std::any> &values,
                             const std::function<std::string(std::any)> &idFunction)
    : name(std::move(name)), values(values), constDomainStorage(true), idFunction(idFunction) {}

std::vector<std::any>::iterator DomainStorage::begin() { return values.begin(); }

std::vector<std::any>::iterator DomainStorage::end() { return values.end(); }

void DomainStorage::addValues(const std::vector<std::any> &vals, const std::function<std::string(std::any)> &func) {
    values = vals;
    idFunction = func;
}

std::vector<std::any> DomainStorage::getValues() const { return values; }

size_t DomainStorage::size() const { return values.size(); }

bool DomainStorage::empty() const { return values.empty(); }

DomainStorage::DomainStorage(std::string name,
                             std::function<std::vector<std::any>(size_t)> dynamicDomainStorageFunction,
                             const std::function<std::string(std::any)> &idFunction,
                             std::function<bool(size_t, std::any)> validityCondition)
    : name(std::move(name)), dynamicDomainStorageFunction(std::move(dynamicDomainStorageFunction)),
      constDomainStorage(false), validityCondition(std::move(validityCondition)), idFunction(idFunction) {}

std::vector<std::any>::iterator DomainStorage::updateDynamicDomain(size_t stepNumber) {
    if (!constDomainStorage)
        values = dynamicDomainStorageFunction(stepNumber);
    return begin();
}

void DomainStorage::reset() {
    if (!constDomainStorage)
        values.clear();
}
bool DomainStorage::isConstDomain() const { return constDomainStorage; }

const std::function<bool(size_t, std::any)> &DomainStorage::getValidityCondition() const {
    return validityCondition.value();
}

const std::function<std::string(std::any)> &DomainStorage::getIdFunction() const { return idFunction; }
