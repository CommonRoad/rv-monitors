#pragma once

#include "domain.h"
#include <map>
#include <string>

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
    DomainCollection(const Domain &domain);

    /**
     * Third constructor.
     *
     * @param domains Domains.
     */
    DomainCollection(const std::vector<Domain> &domains);

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
    void addDomain(const Domain &domain);

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
    [[nodiscard]] Domain &getDomain(const std::string &name);

  private:
    std::map<std::string, Domain> domains; //**< Domains. */

};
