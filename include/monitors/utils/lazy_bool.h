#pragma once

#include <functional>
#include <ostream>

/**
 * Boolean whose value is only initialized when accessed.
 */
class lazy_bool {

  private:
    mutable bool value{false};
    mutable bool initialized{false};
    std::function<bool()> valueFetcher{};

  public:
    lazy_bool() : initialized(true){};

    /**
     * Initialize non-lazy.
     * @param value
     */
    explicit lazy_bool(bool value);

    /**
     * Constructor with functions.
     * @param valueFetcher
     */
    explicit lazy_bool(std::function<bool()> valueFetcher);

    lazy_bool(const lazy_bool &) = default;
    lazy_bool(lazy_bool &&) = default;

    lazy_bool &operator=(const lazy_bool &) = default;
    lazy_bool &operator=(lazy_bool &&) = default;

    /**
     * Implicit cast to bool that fetches the value if necessary.
     * @return
     */
    operator bool() { return get(); }

    /**
     * Fetches the value.
     * @return value
     */
    bool get() const;

    /**
     * Const to_string that indicates the value or not-initialized.
     * @return value
     */
    [[nodiscard]] std::string to_string() const;

    friend std::ostream &operator<<(std::ostream &os, const lazy_bool &aBool);
};
