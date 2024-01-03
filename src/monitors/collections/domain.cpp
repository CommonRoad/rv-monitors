#include "monitors/collections/domain.h"
#include <utility>

DomainStorage::DomainStorage(std::string name) : name(std::move(name)), constDomainStorage(true) {}

std::string DomainStorage::getName() const { return name; }

DomainStorage::DomainStorage(std::string name, const std::vector<std::any> &values)
    : name(std::move(name)), values(values), constDomainStorage(true) {}

std::vector<std::any>::iterator DomainStorage::begin() { return values.begin(); }

std::vector<std::any>::iterator DomainStorage::end() { return values.end(); }

void DomainStorage::addValue(const std::any &val) { values.push_back(val); }

void DomainStorage::addValues(const std::vector<std::any> &vals) {
    for (const auto &val : vals)
        values.push_back(val);
}

std::vector<std::any> DomainStorage::getValues() const { return values; }

size_t DomainStorage::size() const { return values.size(); }

bool DomainStorage::empty() const { return values.empty(); }

DomainStorage::DomainStorage(std::string name,
                             std::function<std::vector<std::any>(size_t)> dynamicDomainStorageFunction)
    : name(std::move(name)), dynamicDomainStorageFunction(std::move(dynamicDomainStorageFunction)),
      constDomainStorage(false) {}

std::vector<std::any>::iterator DomainStorage::updateDynamicDomain(size_t stepNumber) {
    if (!constDomainStorage)
        values = dynamicDomainStorageFunction(stepNumber);
    return begin();
}

void DomainStorage::reset() {
    if (!constDomainStorage)
        values.clear();
}
