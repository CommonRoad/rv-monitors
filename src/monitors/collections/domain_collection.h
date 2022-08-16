#pragma once

#include "domain.h"
#include <map>
#include <string>

class DomainCollection {

    /// Domains
    std::map<std::string, Domain> domains;

  public:

    /// First constructor
    DomainCollection() = default;

    /// Second constructor
    DomainCollection(const Domain &domain);

    /// Third constructor
    DomainCollection(const std::vector<Domain> &domains);

    /**
     * Returns number of domains.
     *
     * @return Size
     */
    [[nodiscard]] size_t size() const;

    /**
     * Adds a collections.
     *
     * @param domain Domain
     */
    void addDomain(const Domain &domain);

    /**
     * Removes a collections.
     *
     * @param name Name
     */
    void removeDomain(const std::string &name);

    /**
     * Returns a requested domain by name. If domain is undefined, runtime error is thrown.
     *
     * @param name Name
     * @return Domain
     */
    [[nodiscard]] Domain &getDomain(const std::string &name);

};
