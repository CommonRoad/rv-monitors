#pragma once

#include "string"
#include "vector"
#include <any>
#include <functional>

class DomainStorage {
  public:
    /**
     * Constructor with name and values as arguments.
     *
     * @param name Name of DomainStorage.
     * @param dynamicDomainStorageFunction Function to create dynamic domain.
     */
    DomainStorage(std::string name, std::function<std::vector<std::any>(size_t)> dynamicDomainStorageFunction);
    /**
     * Constructor with name and values as arguments.
     *
     * @param name Name of DomainStorage.
     * @param values Values of DomainStorage.
     */
    DomainStorage(std::string name, const std::vector<std::any> &values);

    /**
     * Constructor with only name as argument.
     *
     * @param name Name of DomainStorage.
     */
    DomainStorage(std::string name);

    /**
     * Default constructor;
     */
    DomainStorage() = default;

    /**
     * Returns size of DomainStorage.
     *
     * @return Size.
     */
    [[nodiscard]] size_t size() const;

    /**
     * Returns whether DomainStorage is empty.
     *
     * @return Boolean indicating whether DomainStorage is empty.
     */
    [[nodiscard]] bool empty() const;

    /**
     * Returns name of DomainStorage.
     *
     * @return Name.
     */
    [[nodiscard]] std::string getName() const;

    /**
     * Returns iterator pointing to beginning.
     *
     * @return Iterator.
     */
    [[nodiscard]] std::vector<std::any>::iterator begin();

    /**
     * Returns iterator pointing to ending.
     *
     * @return Iterator.
     */
    [[nodiscard]] std::vector<std::any>::iterator end();

    /**
     * Adds value to domain.
     *
     * @param val Value.
     */
    void addValue(const std::any &val);

    /**
     * Adds values to domain.
     *
     * @param values Values.
     */
    void addValues(const std::vector<std::any> &vals);

    /**
     * Getter for values.
     *
     * @return List of DomainStorage values.
     */
    [[nodiscard]] std::vector<std::any> getValues() const;

    /**
     * Updates dynamic domain by calling registered domain function.
     *
     * @param stepNumber Step number of interest.
     * @return Iterator to beginning.
     */
    std::vector<std::any>::iterator updateDynamicDomain(size_t stepNumber);

    /**
     * Resets dynamic domain.
     */
    void reset();

  private:
    std::string name;                                                          //**< Name of DomainStorage. */
    std::vector<std::any> values;                                              //**< Values of DomainStorage. */
    std::function<std::vector<std::any>(size_t)> dynamicDomainStorageFunction; //**< Function for dynamic domain. */
    bool constDomainStorage; //**< Boolean indicating whether domain is const or dynamic. */
};
