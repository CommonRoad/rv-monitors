#pragma once

#include <cstddef>
#include <optional>
#include <stdexcept>

/**
 * Verdict representing result of monitor.
 */
struct Verdict {
    /**
     * Constructor.
     *
     * @param timeStamp Time stamp of verdict.
     * @param value Boolean value of verdict.
     */
    Verdict(size_t timeStamp, bool value) : timeStamp(timeStamp), value(value) {}

    /**
     *  Constructor.
     *
     * @param value Boolean value of verdict.
     */
    Verdict(bool value) : value(value) {}

    std::optional<size_t> timeStamp; //**< Time stamp of verdict. */
    bool value;                      //**< Boolean value of verdict. */

    [[nodiscard]] std::string to_string() const {
        return value ? "true" : "false" + (timeStamp.has_value() ? "[" + std::to_string(timeStamp.value()) + "]" : "");
    }

    /**
     * Equality operator for two verdicts.
     *
     * @param verdict Verdict to which base verdict should be compared with.
     * @return Boolean value indicating equality.
     */
    bool operator==(const Verdict &verdict) const {
        return (verdict.timeStamp.has_value() and this->timeStamp.has_value() and
                this->timeStamp == verdict.timeStamp and this->value == verdict.value) or
               (!verdict.timeStamp.has_value() and !this->timeStamp.has_value() and this->value == verdict.value);
    }

    /**
     * Negation operator for verdict.
     *
     * @return Negated verdict.
     */
    Verdict operator!() const {
        if (this->timeStamp.has_value())
            return {this->timeStamp.value(), !this->value};
        else
            return {!this->value};
    }

    /**
     * Logical and operator for verdict.
     *
     * @param verdict Verdict on right side of logical and operator.
     * @return Verdict representing result of logical and operation on two verdicts.
     */
    Verdict operator&&(const Verdict &verdict) const {
        if (!this->timeStamp.has_value() or !verdict.timeStamp.has_value())
            return {false};
        if (this->timeStamp.value() != verdict.timeStamp.value())
            throw std::runtime_error("Verdict logical and: time steps do not match. Time step this: " +
                                     std::to_string(this->timeStamp.value()) +
                                     ". Time step w: " + std::to_string(verdict.timeStamp.value()));
        return {this->timeStamp.value(), this->value && verdict.value};
    }

    /**
     * Logical or operator for verdict.
     *
     * @param verdict Verdict on right side of logical or operator.
     * @return Verdict representing result of logical or operation on two verdicts.
     */
    Verdict operator||(const Verdict &verdict) const {
        if (!this->timeStamp.has_value() or !verdict.timeStamp.has_value())
            return {false};
        if (this->timeStamp.value() != verdict.timeStamp.value())
            throw std::runtime_error("Verdict logical or: time steps do not match. Time step this: " +
                                     std::to_string(this->timeStamp.value()) +
                                     ". Time step w: " + std::to_string(verdict.timeStamp.value()));
        return {this->timeStamp.value(), this->value || verdict.value};
    }
};
