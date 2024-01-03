#include "monitors/collections/domain_collection.h"
#include <stdexcept>

DomainCollection::DomainCollection(const DomainStorage &domain) {
    domains.insert(std::make_pair(domain.getName(), domain));
}

DomainCollection::DomainCollection(const std::vector<DomainStorage> &domains) {
    for (const DomainStorage &domain : domains)
        this->domains.insert(std::make_pair(domain.getName(), domain));
}

size_t DomainCollection::size() const { return domains.size(); }

void DomainCollection::addDomain(const DomainStorage &domain) {
    domains.insert(std::make_pair(domain.getName(), domain));
}

void DomainCollection::removeDomain(const std::string &name) { domains.erase(name); }

DomainStorage &DomainCollection::getDomain(const std::string &name) {
    std::map<std::string, DomainStorage>::iterator iter{domains.find(name)};
    if (iter != domains.end())
        return iter->second;
    else
        throw std::runtime_error("Undefined domain " + name + "!");
}
