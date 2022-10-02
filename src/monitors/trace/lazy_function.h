#pragma once

#include <functional>
#include <type_traits>
#include <ostream>

template <typename T> struct is_std_array : std::false_type{};
template < typename T, std::size_t N>
struct is_std_array<std::array<T, N> > : std::true_type { };

/**
 * Function whose value is only initialized when accessed.
 */
template <class T> class LazyFunction {
    static_assert((std::is_integral<T>::value or is_std_array<T>::value), "T must be integral or array.");
  private:
    mutable T value{false};
    mutable bool initialized{false};
    std::function<T()> valueFetcher{};

  public:
    LazyFunction() : initialized(true){};

    /**
     * Initialize non-lazy.
     * @param value
     */
    explicit LazyFunction(T value);

    /**
     * Constructor with functions.
     * @param valueFetcher
     */
    LazyFunction(std::function<T()> valueFetcher);

    LazyFunction(const LazyFunction &) = default;
    LazyFunction(LazyFunction &&) noexcept = default;
    LazyFunction &operator=(const LazyFunction &) = default;
    LazyFunction &operator=(LazyFunction &&) noexcept = default;

    /**
     * Implicit cast to bool that fetches the value if necessary.
     * @return
     */
    operator bool();

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

    friend std::ostream &operator<<(std::ostream &oss, const LazyFunction<T> &lazy){
        return oss << lazy.to_string();
    }
};
