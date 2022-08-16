#include "domain_collection.h"

DomainCollection::DomainCollection(const Domain &domain) {
    domains.insert(std::make_pair(domain.getName(), domain));
}

DomainCollection::DomainCollection(const std::vector<Domain> &domains) {
    for (const Domain &domain : domains)
        this->domains.insert(std::make_pair(domain.getName(), domain));
}

size_t DomainCollection::size() const { return domains.size(); }

void DomainCollection::addDomain(const Domain &domain) {
    domains.insert(std::make_pair(domain.getName(), domain));
}

void DomainCollection::removeDomain(const std::string &name) { domains.erase(name); }

Domain &DomainCollection::getDomain(const std::string &name) {
    std::map<std::string, Domain>::iterator iter{domains.find(name)};
    if (iter != domains.end())
        return iter->second;
    else
        throw std::runtime_error("Undefined domain " + name + "!");
}
