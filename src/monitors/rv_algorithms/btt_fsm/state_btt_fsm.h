#pragma once

#include "decision.h"
#include "monitors/trace/event.h"
#include "node.h"
#include <cstdlib>
#include <memory>
#include <string>

/**
 * Models a Node in a BTT which is a state, i.e. there is no left or right as in Decision. It contains an Id and the
 * (non-)terminal BTT for this state.
 */
class StateBttFsm : public node {
  private:
    size_t id;
    std::shared_ptr<node> nonTerminal;
    std::shared_ptr<node> terminal;

  public:
    /**
     * Constructor.
     * @param id Id.
     * @param nonTerminal BTT for non-terminal events.
     * @param terminal BTT for terminal events.
     */
    StateBttFsm(size_t id, std::shared_ptr<node> nonTerminal, std::shared_ptr<node> terminal);

    /**
     * Constructor, terminal and nonTerminal stay empty.
     * @param id Id.
     */
    explicit StateBttFsm(size_t id);

    /**
     * Getter for id.
     * @return Id.
     */
    [[nodiscard]] size_t getId() const;

    /**
     * Getter for nonTerminal.
     * @return BTT for non-terminal event.
     */
    [[nodiscard]] const std::shared_ptr<node> &getNonTerminal() const;

    /**
     * Getter for terminal.
     * @return BTT for terminal event.
     */
    [[nodiscard]] const std::shared_ptr<node> &getTerminal() const;

    /**
     * Setter for id.
     * @param id Id.
     */
    void setId(size_t id);

    /**
     * Setter for nonTerminal.
     * @param nonTerminal BTT for non-terminal event.
     */
    void setNonTerminal(const std::shared_ptr<node> &nonTerminal);

    /**
     * Setter for terminal.
     * @param nonTerminal BTT for terminal event.
     */
    void setTerminal(const std::shared_ptr<node> &terminal);

    /**
     * Decides which state is next during evaluation.
     * @param event Current event to be evaluated.
     * @return Id of state after event has been evaluated.
     */
    [[nodiscard]] size_t nextState(Event &event);

    /**
     * Helper function for nextState(Event& event).
     * @param event Current event to be evaluated.
     * @return Id of state after event has been evaluated.
     */
    [[nodiscard]] size_t nextState(Event &event, const std::shared_ptr<Decision> &decision);

    /**
     * to_string() for StateBttFsm.
     * @return String representation of this StateBttFsm.
     */
    [[nodiscard]] std::string to_string() const override;
};
