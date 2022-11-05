#include "quote_quantifier.h"

QuoteInterval::QuoteInterval(float min, float max)
    : min(min), max(max)
// Constructor
{}

float QuoteInterval::getMin() const
// Returns minimum of probability.
{
    return min;
}

float QuoteInterval::getMax() const
// Returns maximum of probability.
{
    return max;
}

QuoteQuantifier::QuoteQuantifier(std::shared_ptr<Expression> &left, std::vector<Variable> &variables,
                                 QuoteInterval &quoteInterval)
    : Unary(quoteSymbol, left), FirstOrderOperator(variables), quoteInterval(quoteInterval)
// First constructor
{}

QuoteQuantifier::QuoteQuantifier(std::shared_ptr<Expression> &left, Variable &variable, QuoteInterval &quoteInterval)
    : Unary(quoteSymbol, left), FirstOrderOperator(variable), quoteInterval(quoteInterval)
// Second constructor
{}

std::string QuoteQuantifier::to_string() const {
    std::string str = getSymbol() + "[" + std::to_string(getQuoteInterval().getMin()) + ", " +
                      std::to_string(getQuoteInterval().getMax()) + "] ";

    str += FirstOrderOperator::to_string();

    str += getLeft()->to_string();

    return str;
}

std::shared_ptr<Expression> QuoteQuantifier::clone() { return std::make_shared<QuoteQuantifier>(*this); }

std::shared_ptr<Expression> QuoteQuantifier::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitQuoteQuantifier(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> QuoteQuantifier::accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                       bool connectedEval) {
    return visitor.visitQuoteQuantifier(*this, parentId, connectedEval);
}

std::shared_ptr<HydraMonitor> QuoteQuantifier::accept(HydraCreationVisitor &visitor) { return nullptr; }

const QuoteInterval &QuoteQuantifier::getQuoteInterval() const
// Returns probability interval
{
    return quoteInterval;
}
