#pragma once

#include <string>

/**
 * Parent class for Decision and StateBttFsm since we can have both types in a BTT:
 */
class node {
  public:
    /**
     * Dummy to_string(), gets overwritten in child classes Decision and StateBttFsm.
     * @return String representation of the Node.
     */
    [[nodiscard]] virtual std::string to_string() const = 0;
};