#include "until.h"

std::shared_ptr<Expression> Until::clone() { return std::make_shared<Until>(*this); }

std::shared_ptr<Expression> Until::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitUntil(*this, trace, index);
}

std::shared_ptr<HydraMonitor> Until::accept(HydraCreationVisitor &visitor, size_t *maxId,
                                            std::map<std::string, std::shared_ptr<HydraMonitor>> *baseline,
                                            size_t parentId) {
    return visitor.visitUntil(*this, maxId, baseline, parentId);
}
