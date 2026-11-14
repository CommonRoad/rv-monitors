#include "monitors/collections/domain.h"
#include <utility>

DomainStorage::DomainStorage(std::string name) : name(std::move(name)) {}

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

void DomainStorage::setExpectedSize(const size_t size) { expectedSize = size; }

size_t DomainStorage::getExpectedSize() const { return expectedSize; }

size_t DomainStorage::estimatedSize() const {
    if (constDomainStorage or !values.empty())
        return values.size();
    return expectedSize;
}

DomainStorage::DomainStorage(std::string name,
                             std::shared_ptr<IFunction<std::vector<std::any>>> dynamicDomainStorageFunction,
                             const std::function<std::string(std::any)> &idFunction,
                             std::function<bool(size_t, std::any)> validityCondition)
    : name(std::move(name)), dynamicDomainStorageFunction(std::move(dynamicDomainStorageFunction)),
      constDomainStorage(false), validityCondition(std::move(validityCondition)), idFunction(idFunction) {}

std::vector<std::any>::iterator DomainStorage::updateDynamicDomain(size_t timeStep,
                                                                   const std::vector<DomainValue> &domVals,
                                                                   ConstantCollection constants) {
    if (!constDomainStorage) {
        std::vector<DomainValue> args;
        for (const auto &arg : dynamicDomainStorageFunction->getArgumentNames()) {
            if (arg == "stepNumber") {
                args.emplace_back("stepNumber", timeStep, true,
                                  [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); });
                continue;
            }
            for (const auto &val : domVals)
                if (arg == val.getVariable()) {
                    args.push_back(val);
                    break;
                }
            if (constants.contains(arg)) {
                auto val{constants.getConstant(arg)};
                args.emplace_back(val.getName(), val.getValue(), true, val.getIdFunction());
            }
        }
        values = dynamicDomainStorageFunction->getFunction(args)();
    }
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
