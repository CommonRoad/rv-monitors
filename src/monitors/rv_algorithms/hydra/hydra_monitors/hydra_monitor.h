//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once
#include "monitors/utils/utils.h"
#include <optional>

class HydraMonitor {
  public:
    /**
     *
     * @param formula
     */
    HydraMonitor(Formula formula);

    HydraMonitor(Formula formula, const size_t &from, const size_t &to,
                 const std::shared_ptr<HydraMonitor> &sub_monitor);

    /**
     *
     * @return
     */

    virtual Verdict step() = 0;

    virtual void setTrace(const Trace &tr) = 0;

    /**
     *
     * @return
     */
    const Formula &getFormula() const;

    const std::optional<size_t> &getFrom() const;

    const std::optional<size_t> &getTo() const;

    const std::optional<std::shared_ptr<HydraMonitor>> &getSubMonitor() const;

    const std::optional<Verdict> &getCurrentVerdict() const;

  protected:
    Formula formula;
    std::optional<Trace> trace;
    std::optional<size_t> from, to;
    std::optional<Verdict> currentVerdict;
    std::optional<std::shared_ptr<HydraMonitor>> subMonitor;
};
