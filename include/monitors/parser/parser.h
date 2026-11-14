#pragma once

#include "monitors/formula/formula.h"
#include <string>

/**
 * Parser class to parse and directly build the formula.
 */
class Parser {
  public:
    /**
     * Transforms a formula string into a formula object.
     * @param formula Formula string to be parsed.
     * @return Formula object.
     */
    [[nodiscard]] static Formula parse(const std::string &formula, const std::string &formulaId = "");

    /**
     * Transforms a Hydra2-style log string into a log (trace) object.
     * @param formula Hydra2-style log string to be parsed.
     * @return Trace object.
     */
    [[nodiscard]] static std::shared_ptr<Trace> parse_log(const std::string &trace);
};
