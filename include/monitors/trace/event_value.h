#pragma once

#include <functional>
#include <ostream>

/**
 * Boolean whose value is only initialized when accessed.
 */
template <typename T> class EventValue {

  private:
    mutable T value{};
    mutable bool initialized{false};
    std::function<T()> valueFetcher{};

  public:
    EventValue() : initialized(true) {};

    /**
     * Initialize non-lazy.
     * @param value
     */
    explicit EventValue(T value);

    /**
     * Constructor with functions.
     * @param valueFetcher
     */
    explicit EventValue(std::function<T()> valueFetcher);

    EventValue(const EventValue &) = default;
    EventValue(EventValue &&) noexcept = default;

    EventValue &operator=(const EventValue &) = default;
    EventValue &operator=(EventValue &&) noexcept = default;

    /**
     * Implicit cast to bool that fetches the value if necessary.
     * @return
     */
    operator T() const { return get(); }

    /**
     * Fetches the value.
     * @return value
     */
    T get() const;

    /**
     * Const to_string that indicates the value or not-initialized.
     * @return value
     */
    [[nodiscard]] std::string to_string() const;

    friend std::ostream &operator<<(std::ostream &oss, const EventValue<T> &val) { return oss << val.to_string(); };
};

template <typename T> EventValue<T>::EventValue(std::function<T()> valueFetcher)
    : valueFetcher(std::move(valueFetcher)) {}

template <typename T> T EventValue<T>::get() const {
    if (not initialized) {
        value = valueFetcher();
        initialized = true;
    }
    return value;
}

template <typename T> std::string EventValue<T>::to_string() const {
    if (initialized) {
        return value ? "true" : "false";
    }
    return "Not initialized";
}

template <typename T> EventValue<T>::EventValue(T value) : value(value), initialized(true) {}
