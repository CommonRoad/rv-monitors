#include "and.h"

std::shared_ptr<Expression> And::clone() { return std::make_shared<And>(*this); }

std::shared_ptr<Expression> And::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitAnd(*this, trace, index);
}
std::shared_ptr<HydraMonitor> And::accept(HydraCreationVisitor &visitor, size_t *maxId,
                                          std::map<std::string, std::shared_ptr<HydraMonitor>> *baseline,
                                          size_t parentId) {
    return visitor.visitAnd(*this, maxId, baseline, parentId);
}
