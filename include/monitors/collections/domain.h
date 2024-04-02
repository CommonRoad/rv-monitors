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
     * @param idFunction Function to extract unique ID of domain value.
     * @param validityCondition Boolean functions which checks whether dynamic domain value is valid at step number.
     */
    DomainStorage(std::string name, std::function<std::vector<std::any>(size_t)> dynamicDomainStorageFunction,
                  const std::function<std::string(std::any)> &idFunction,
                  std::function<bool(size_t, std::any)> validityCondition);
    /**
     * Constructor with name and values as arguments.
     *
     * @param name Name of DomainStorage.
     * @param values Values of DomainStorage.
     * @param idFunction Function to extract unique ID of domain value.
     */
    DomainStorage(std::string name, const std::vector<std::any> &values,
                  const std::function<std::string(std::any)> &idFunction);

    /**
     * Constructor with only name as argument.
     *
     * @param name Name of DomainStorage.
     */
    explicit DomainStorage(std::string name);

    /**
     * Default constructor;
     */
    DomainStorage() = default;

    /**
     * Returns size of DomainStorage.
     *
     * @return Size of domain.
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
     * Adds values to domain. Deletes previously added values.
     *
     * @param values Values.
     * @param func ID function for values.
     */
    void addValues(const std::vector<std::any> &vals, const std::function<std::string(std::any)> &func);

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

    /**
     * Checks whether domain is constant.
     *
     * @return Boolean indicating whether domain is const.
     */
    [[nodiscard]] bool isConstDomain() const;

    /**
     * Getter for ID function.
     *
     * @return ID extraction function.
     */
    const std::function<std::string(std::any)> &getIdFunction() const;

    /**
     * Getter for validity condition function.
     *
     * @return Validity condition function.
     */
    const std::function<bool(size_t, std::any)> &getValidityCondition() const;

  private:
    mutable std::string name;     //**< Name of DomainStorage. */
    std::vector<std::any> values; //**< Values of DomainStorage. */
    mutable std::function<std::vector<std::any>(size_t)>
        dynamicDomainStorageFunction;      //**< Function for dynamic domain. */
    mutable bool constDomainStorage{true}; //**< Boolean indicating whether domain is const or dynamic. */
    mutable std::optional<std::function<bool(size_t, std::any)>>
        validityCondition;                                   //**< Validity condition function. */
    mutable std::function<std::string(std::any)> idFunction; //**< ID function. */
};
