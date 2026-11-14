#pragma once

#include "string"
#include "vector"
#include <any>

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
    DomainStorage(std::string name, std::shared_ptr<IFunction<std::vector<std::any>>> dynamicDomainStorageFunction,
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
     * Sets the expected number of values of a dynamic domain. It is used to estimate the evaluation cost and
     * satisfaction probability of quantifiers over the domain as long as its values are not known yet, e.g., when the
     * monitor is created.
     *
     * @param size Expected number of values.
     */
    void setExpectedSize(size_t size);

    /**
     * Returns the expected number of values of a dynamic domain (default: 1).
     *
     * @return Expected number of values.
     */
    [[nodiscard]] size_t getExpectedSize() const;

    /**
     * Returns the estimated number of values: the actual number for constant domains and dynamic domains with known
     * values, the expected number for dynamic domains whose values are not known yet.
     *
     * @return Estimated number of values.
     */
    [[nodiscard]] size_t estimatedSize() const;

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
     * @param vals Values.
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
     * @param timeStep Relevant time step of dynamic domain. Can differ from current trace step in case of
     * non-equidistant time step or skipped steps.
     * @param domVals Additional arguments for domain function from parent domains.
     * @param constants List of constant values.
     * @return Iterator to beginning.
     */
    std::vector<std::any>::iterator updateDynamicDomain(size_t timeStep, const std::vector<DomainValue> &domVals = {},
                                                        ConstantCollection constants = {});

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
    mutable std::shared_ptr<IFunction<std::vector<std::any>>>
        dynamicDomainStorageFunction;      //**< Function for dynamic domain. */
    mutable bool constDomainStorage{true}; //**< Boolean indicating whether domain is const or dynamic. */
    mutable std::optional<std::function<bool(size_t, std::any)>>
        validityCondition;                                   //**< Validity condition function. */
    mutable std::function<std::string(std::any)> idFunction; //**< ID function. */
    size_t expectedSize{1}; //**< Expected number of values of a dynamic domain whose values are not known yet. */
};
