#include "log_visitor.h"
#include "monitors/parser/gen/LogBaseVisitor.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"

namespace impl {

using identifier = std::string;

struct LogEntry {
    size_t time;
    std::vector<identifier> true_atomics;
};

struct Log {
    std::vector<LogEntry> entries;
};

antlrcpp::Any LogVisitor::visitLog(LogParser::LogContext *context) {
    std::set<std::string> atomics;
    std::vector<std::vector<std::string>> true_atomics;
    std::vector<size_t> time_steps;

    for (auto& a_entry: context->entry()) {
        LogEntry entry = visit(a_entry).as<LogEntry>();

        time_steps.push_back(entry.time);

        for (auto& atomic: entry.true_atomics) {
            atomics.insert(atomic);
        }

        true_atomics.push_back(entry.true_atomics);
    }

    Trace trace = Utils::createTrace(atomics, true_atomics, time_steps);

    return trace;
}

antlrcpp::Any LogVisitor::visitEntry(LogParser::EntryContext *context) {
    auto time = visit(context->time()).as<size_t>();
    std::vector<identifier> true_atomics;

    for (auto &entry : context->identifier()) {
        auto atomic = visit(entry).as<identifier>();
        true_atomics.push_back(atomic);
    }

    return LogEntry {
      .time = time,
      .true_atomics = true_atomics
    };
}

antlrcpp::Any LogVisitor::visitIdentifier(LogParser::IdentifierContext *context) {
    return context->getText();
}

antlrcpp::Any LogVisitor::visitTime(LogParser::TimeContext *context) {
    return static_cast<size_t>(std::stoi(context->DIGIT()->getText()));
}

};
