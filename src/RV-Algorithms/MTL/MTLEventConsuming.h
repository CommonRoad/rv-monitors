#pragma once

#include <Formula/Formula.h>
#include <Trace/Trace.h>

/**
 * Functions for monitoring a trace using the MTL event consuming algorithm.
 */
namespace MTLEventConsuming {
/**
 * Monitors a formula in MTL or LTL on a trace. The formula can have future and past operators.
 * @param formula Formula.
 * @param trace Trace.
 * @return Formula changed by the event consuming algorithm, should be true or false at the end.
 */
[[nodiscard]] bool eventConsuming(const Formula &formula, Trace &trace);

}; // namespace MTLEventConsuming
