#pragma once

#include "domain.h"
#include <map>

/**
 * Storage for a set of domains.
 */
class DomainCollection {
  public:
    /**
     * First constructor.
     */
    DomainCollection() = default;

    /**
     * Second constructor.
     *
     * @param domain Domain.
     */
    DomainCollection(const DomainStorage &domain);

    /**
     * Third constructor.
     *
     * @param domains Domains.
     */
    DomainCollection(const std::vector<DomainStorage> &domains);

    /**
     * Returns number of stored domains.
     *
     * @return Size of collection.
     */
    [[nodiscard]] size_t size() const;

    /**
     * Adds domain to collection.
     *
     * @param domain Domain.
     */
    void addDomain(const DomainStorage &domain);

    /**
     * Removes domain from collection.
     *
     * @param name Name of domain.
     */
    void removeDomain(const std::string &name);

    /**
     * Returns a requested domain by name. If domain is undefined, runtime error is thrown.
     *
     * @param name Name of domain.
     * @return Domain.
     */
    [[nodiscard]] DomainStorage &getDomain(const std::string &name);

  private:
    std::map<std::string, DomainStorage> domains; //**< Domains. */
};
