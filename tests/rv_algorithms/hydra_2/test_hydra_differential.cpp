// Randomized differential tests for the Hydra2 monitor.
//
// Random first-order MTL formulas and random timed traces are generated with a fixed seed. Every formula set is
// monitored with all eight combinations of the optimizations (sharing of subformulas across and within formulas,
// cost-based reordering, and short-circuit/lazy evaluation) and the verdict at every time step is compared with a
// naive reference evaluator that implements the point-based finite-trace semantics directly. Each monitor instance
// is reused for several traces, so the reset logic is exercised as well. Failing cases are shrunk to a small
// counterexample before they are reported.

#include "hydra_test_utils.h"
#include "monitors/collections/constant_collection.h"
#include "monitors/collections/domain.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra.h"
#include "monitors/utils/utils.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace {

// ---------------------------------------------------------------------------------------------------------------------
// Vocabulary
// ---------------------------------------------------------------------------------------------------------------------

// Propositions of the trace; the atom domain "D" contains exactly these names.
constexpr std::array<const char *, 3> kAtoms{"p", "q", "r"};
// Values of the numeric domain "N" used as predicate arguments.
constexpr std::array<size_t, 3> kNumbers{0, 1, 2};
// Constants usable as predicate arguments: 'k0' = 0 and 'k1' = 1.
constexpr std::array<const char *, 2> kConstants{"k0", "k1"};
// Variables over N (used in predicates) and over D (used as propositions).
constexpr std::array<const char *, 2> kNumVars{"x", "y"};
constexpr std::array<const char *, 2> kAtomVars{"u", "v"};

struct PredicateInfo {
    const char *name;
    size_t arity;
    uint64_t truePercent; // probability that the predicate holds for a random argument tuple
};
constexpr std::array<PredicateInfo, 3> kPredicates{{{"lo", 1, 30}, {"hi", 1, 70}, {"rel", 2, 50}}};

uint64_t splitmix64(uint64_t x) {
    x += 0x9E3779B97F4A7C15ULL;
    x = (x ^ (x >> 30U)) * 0xBF58476D1CE4E5B9ULL;
    x = (x ^ (x >> 27U)) * 0x94D049BB133111EBULL;
    return x ^ (x >> 31U);
}

// Ground truth of predicate `predIdx` at time stamp `timeStamp` for arguments (a, b); deterministic per trace seed.
bool predicateValue(const uint64_t seed, const size_t predIdx, const size_t timeStamp, const size_t a, const size_t b) {
    const uint64_t h{splitmix64(seed ^ splitmix64(predIdx * 1000003ULL + timeStamp * 101ULL + a * 11ULL + b))};
    return h % 100 < kPredicates.at(predIdx).truePercent;
}

// ---------------------------------------------------------------------------------------------------------------------
// Formula representation
// ---------------------------------------------------------------------------------------------------------------------

enum class Op {
    True,
    False,
    Atom,    // proposition p, q, r
    VarAtom, // variable over D used as a proposition
    Pred,    // predicate over variables of N and constants
    Not,
    And,
    Or,
    Implies,
    Next,
    Prev,
    Until,
    Since,
    Future,
    Globally,
    Once,
    Historically,
    Exists,
    Forall,
    Count,
    Quote
};

struct Interval {
    size_t lo{0};
    size_t hi{0};
    bool inf{true};
    bool printed{true}; // false: printed without interval, i.e., [0, inf]

    [[nodiscard]] bool contains(const size_t d) const { return d >= lo and (inf or d <= hi); }

    [[nodiscard]] std::string str() const {
        if (!printed)
            return "";
        return "[" + std::to_string(lo) + ", " + (inf ? std::string("inf") : std::to_string(hi)) + "]";
    }
};

struct Node;
using NodePtr = std::shared_ptr<Node>;

struct Node {
    Op op{Op::True};
    std::string name;              // atom, variable, or predicate name
    size_t predIdx{0};             // index into kPredicates
    std::vector<std::string> args; // predicate arguments: variable names or constant names
    std::vector<NodePtr> ch;       // children
    Interval iv;                   // temporal operators
    std::vector<std::string> vars; // quantified variables
    bool numDomain{true};          // quantifier over N (true) or D (false)
    std::string countOp;           // "<=", "=", ">="
    size_t countNum{0};
    size_t quoteLo{0}; // quote bounds in tenths
    size_t quoteHi{0};
};

std::string quoteText(const size_t tenths) { return tenths >= 10 ? "1.0" : "0." + std::to_string(tenths); }

std::string toString(const NodePtr &n) {
    const auto c = [&n](const size_t i) { return toString(n->ch.at(i)); };
    const auto join = [&n](const std::string &sep) {
        std::string s;
        for (size_t i{0}; i < n->ch.size(); ++i)
            s += (i == 0 ? "" : sep) + std::string("(") + toString(n->ch[i]) + ")";
        return "(" + s + ")";
    };
    const auto varList = [&n]() {
        std::string s;
        for (size_t i{0}; i < n->vars.size(); ++i)
            s += (i == 0 ? "" : ", ") + n->vars[i];
        return s + " in " + (n->numDomain ? "N" : "D");
    };
    switch (n->op) {
    case Op::True:
        return "true";
    case Op::False:
        return "false";
    case Op::Atom:
    case Op::VarAtom:
        return n->name;
    case Op::Pred: {
        std::string s{n->name + "("};
        for (size_t i{0}; i < n->args.size(); ++i) {
            const bool isConst{n->args[i].rfind('k', 0) == 0};
            s += (i == 0 ? "" : ", ") + (isConst ? "'" + n->args[i] + "'" : n->args[i]);
        }
        return s + ")";
    }
    case Op::Not:
        return "!(" + c(0) + ")";
    case Op::And:
        return join(" & ");
    case Op::Or:
        return join(" | ");
    case Op::Implies:
        return join(" -> ");
    case Op::Next:
        return "X" + n->iv.str() + "(" + c(0) + ")";
    case Op::Prev:
        return "P" + n->iv.str() + "(" + c(0) + ")";
    case Op::Future:
        return "F" + n->iv.str() + "(" + c(0) + ")";
    case Op::Globally:
        return "G" + n->iv.str() + "(" + c(0) + ")";
    case Op::Once:
        return "O" + n->iv.str() + "(" + c(0) + ")";
    case Op::Historically:
        return "H" + n->iv.str() + "(" + c(0) + ")";
    case Op::Until:
        return "((" + c(0) + ") U" + n->iv.str() + " (" + c(1) + "))";
    case Op::Since:
        return "((" + c(0) + ") S" + n->iv.str() + " (" + c(1) + "))";
    case Op::Exists:
        return "(E " + varList() + ". (" + c(0) + "))";
    case Op::Forall:
        return "(A " + varList() + ". (" + c(0) + "))";
    case Op::Count:
        return "(C" + n->countOp + std::to_string(n->countNum) + " " + varList() + ". (" + c(0) + "))";
    case Op::Quote:
        return "(Q[" + quoteText(n->quoteLo) + ", " + quoteText(n->quoteHi) + "] " + varList() + ". (" + c(0) + "))";
    }
    return "";
}

void collectFreeVars(const NodePtr &n, std::set<std::string> &bound, std::set<std::string> &free) {
    if (n->op == Op::VarAtom and bound.count(n->name) == 0)
        free.insert(n->name);
    if (n->op == Op::Pred)
        for (const auto &a : n->args)
            if (a.rfind('k', 0) != 0 and bound.count(a) == 0)
                free.insert(a);
    std::vector<std::string> added;
    for (const auto &v : n->vars)
        if (bound.insert(v).second)
            added.push_back(v);
    for (const auto &child : n->ch)
        collectFreeVars(child, bound, free);
    for (const auto &v : added)
        bound.erase(v);
}

std::set<std::string> freeVars(const NodePtr &n) {
    std::set<std::string> bound;
    std::set<std::string> free;
    collectFreeVars(n, bound, free);
    return free;
}

// True if every quantifier uses all of its variables (counting and quote quantifiers depend on their number).
bool quantifiedVariablesUsed(const NodePtr &n) {
    if (!n->vars.empty()) {
        const auto used{freeVars(n->ch.at(0))};
        for (const auto &v : n->vars)
            if (used.count(v) == 0)
                return false;
    }
    for (const auto &child : n->ch)
        if (!quantifiedVariablesUsed(child))
            return false;
    return true;
}

size_t nodeCount(const NodePtr &n) {
    size_t c{1};
    for (const auto &child : n->ch)
        c += nodeCount(child);
    return c;
}

// ---------------------------------------------------------------------------------------------------------------------
// Random generation
// ---------------------------------------------------------------------------------------------------------------------

class Generator {
  public:
    explicit Generator(const uint64_t seed) : rng(seed) {}

    size_t uniform(const size_t lo, const size_t hi) { return std::uniform_int_distribution<size_t>(lo, hi)(rng); }
    bool chance(const double p) { return std::bernoulli_distribution(p)(rng); }
    uint64_t next() { return rng(); }

    template <typename T> const T &pick(const std::vector<T> &v) { return v.at(uniform(0, v.size() - 1)); }

    NodePtr formula(const size_t depth) {
        Scope scope;
        return gen(depth, scope);
    }

  private:
    struct Scope {
        std::vector<std::string> numVars;
        std::vector<std::string> atomVars;
    };

    std::mt19937_64 rng;

    Interval interval() {
        Interval iv;
        if (chance(0.15)) {
            iv.printed = false;
            return iv;
        }
        iv.lo = uniform(0, 2);
        iv.inf = chance(0.25);
        iv.hi = iv.lo + uniform(0, 3);
        return iv;
    }

    NodePtr leaf(const Scope &scope) {
        auto n{std::make_shared<Node>()};
        std::vector<std::pair<int, double>> options{{0, 4.0}, {1, 0.4}, {2, 3.0}, {3, 1.5}};
        if (!scope.atomVars.empty())
            options.emplace_back(4, 3.0);
        double total{0.0};
        for (const auto &o : options)
            total += o.second;
        double r{std::uniform_real_distribution<double>(0.0, total)(rng)};
        int choice{options.back().first};
        for (const auto &o : options) {
            if (r < o.second) {
                choice = o.first;
                break;
            }
            r -= o.second;
        }
        switch (choice) {
        case 0:
            n->op = Op::Atom;
            n->name = kAtoms.at(uniform(0, kAtoms.size() - 1));
            break;
        case 1:
            n->op = chance(0.5) ? Op::True : Op::False;
            break;
        case 2: // unary predicate
        case 3: // binary predicate
        {
            n->op = Op::Pred;
            n->predIdx = choice == 2 ? uniform(0, 1) : 2;
            n->name = kPredicates.at(n->predIdx).name;
            for (size_t a{0}; a < kPredicates.at(n->predIdx).arity; ++a) {
                if (!scope.numVars.empty() and chance(0.75))
                    n->args.push_back(pick(scope.numVars));
                else
                    n->args.emplace_back(kConstants.at(uniform(0, kConstants.size() - 1)));
            }
            break;
        }
        default:
            n->op = Op::VarAtom;
            n->name = pick(scope.atomVars);
        }
        return n;
    }

    // Leaf that is guaranteed to mention variable `var`.
    static NodePtr leafUsing(const std::string &var, const bool numDomain, Generator &g) {
        auto n{std::make_shared<Node>()};
        if (numDomain) {
            n->op = Op::Pred;
            n->predIdx = g.uniform(0, 1);
            n->name = kPredicates.at(n->predIdx).name;
            n->args = {var};
        } else {
            n->op = Op::VarAtom;
            n->name = var;
        }
        return n;
    }

    static bool mentions(const NodePtr &n, const std::string &var) { return freeVars(n).count(var) > 0; }

    NodePtr gen(const size_t depth, Scope &scope) {
        if (depth == 0 or chance(0.2))
            return leaf(scope);

        enum Kind { kNot, kAnd, kOr, kImpl, kNext, kPrev, kUntil, kSince, kF, kG, kO, kH, kQuant };
        static const std::vector<std::pair<Kind, double>> weights{
            {kNot, 2.0},   {kAnd, 3.0}, {kOr, 3.0}, {kImpl, 1.0}, {kNext, 1.5}, {kPrev, 1.5}, {kUntil, 2.0},
            {kSince, 2.0}, {kF, 1.5},   {kG, 1.5},  {kO, 1.5},    {kH, 1.5},    {kQuant, 4.5}};
        double total{0.0};
        for (const auto &w : weights)
            total += w.second;
        double r{std::uniform_real_distribution<double>(0.0, total)(rng)};
        Kind kind{weights.back().first};
        for (const auto &w : weights) {
            if (r < w.second) {
                kind = w.first;
                break;
            }
            r -= w.second;
        }

        auto n{std::make_shared<Node>()};
        switch (kind) {
        case kNot:
            n->op = Op::Not;
            n->ch.push_back(gen(depth - 1, scope));
            return n;
        case kAnd:
        case kOr: {
            n->op = kind == kAnd ? Op::And : Op::Or;
            const size_t arity{uniform(2, 3)};
            for (size_t i{0}; i < arity; ++i)
                n->ch.push_back(gen(depth - 1, scope));
            return n;
        }
        case kImpl:
            n->op = Op::Implies;
            n->ch.push_back(gen(depth - 1, scope));
            n->ch.push_back(gen(depth - 1, scope));
            return n;
        case kNext:
        case kPrev:
        case kF:
        case kG:
        case kO:
        case kH: {
            static const std::map<Kind, Op> ops{{kNext, Op::Next},  {kPrev, Op::Prev}, {kF, Op::Future},
                                                {kG, Op::Globally}, {kO, Op::Once},    {kH, Op::Historically}};
            n->op = ops.at(kind);
            n->iv = interval();
            n->ch.push_back(gen(depth - 1, scope));
            return n;
        }
        case kUntil:
        case kSince:
            n->op = kind == kUntil ? Op::Until : Op::Since;
            n->iv = interval();
            n->ch.push_back(gen(depth - 1, scope));
            n->ch.push_back(gen(depth - 1, scope));
            return n;
        case kQuant:
            break;
        }

        // Quantifier: choose a domain with at least one unbound variable.
        n->numDomain = chance(0.6);
        auto &boundVars{n->numDomain ? scope.numVars : scope.atomVars};
        std::vector<std::string> freeNames;
        for (const auto *v : (n->numDomain ? kNumVars : kAtomVars))
            if (std::find(boundVars.begin(), boundVars.end(), v) == boundVars.end())
                freeNames.emplace_back(v);
        if (freeNames.empty())
            return gen(depth, scope);
        const size_t numNewVars{freeNames.size() > 1 and chance(0.2) ? 2UL : 1UL};
        for (size_t i{0}; i < numNewVars; ++i)
            n->vars.push_back(freeNames.at(i));

        const double q{std::uniform_real_distribution<double>(0.0, 1.0)(rng)};
        if (q < 0.35) {
            n->op = Op::Exists;
        } else if (q < 0.7) {
            n->op = Op::Forall;
        } else if (q < 0.88) {
            n->op = Op::Count;
            static const std::vector<std::string> cops{"<=", "=", ">="};
            n->countOp = pick(cops);
            n->countNum = uniform(0, 3);
        } else {
            n->op = Op::Quote;
            static const std::vector<size_t> bounds{0, 2, 4, 5, 6, 8, 10};
            size_t a{pick(bounds)};
            size_t b{pick(bounds)};
            if (a > b)
                std::swap(a, b);
            n->quoteLo = a;
            n->quoteHi = b;
        }

        for (const auto &v : n->vars)
            boundVars.push_back(v);
        NodePtr body{gen(depth - 1, scope)};
        // Make sure that every quantified variable is used in the body; the counting and quote quantifiers
        // depend on the number of variable combinations.
        for (const auto &v : n->vars) {
            if (mentions(body, v))
                continue;
            auto combined{std::make_shared<Node>()};
            combined->op = chance(0.5) ? Op::And : Op::Or;
            combined->ch = {body, leafUsing(v, n->numDomain, *this)};
            body = combined;
        }
        for (size_t i{0}; i < n->vars.size(); ++i)
            boundVars.pop_back();
        n->ch.push_back(body);
        return n;
    }
};

// ---------------------------------------------------------------------------------------------------------------------
// Traces
// ---------------------------------------------------------------------------------------------------------------------

struct TraceData {
    std::vector<size_t> timeStamps;
    std::vector<std::array<bool, kAtoms.size()>> atoms;
    uint64_t predicateSeed{0};

    [[nodiscard]] size_t size() const { return timeStamps.size(); }

    [[nodiscard]] std::string str() const {
        std::ostringstream s;
        for (size_t i{0}; i < size(); ++i) {
            s << "  i=" << i << " ts=" << timeStamps[i] << " {";
            bool first{true};
            for (size_t a{0}; a < kAtoms.size(); ++a)
                if (atoms[i][a]) {
                    s << (first ? "" : ",") << kAtoms.at(a);
                    first = false;
                }
            s << "}";
            for (size_t pIdx{0}; pIdx < kPredicates.size(); ++pIdx) {
                s << " " << kPredicates.at(pIdx).name << "=";
                for (const size_t a : kNumbers) {
                    if (kPredicates.at(pIdx).arity == 1)
                        s << predicateValue(predicateSeed, pIdx, timeStamps[i], a, 0);
                    else
                        for (const size_t b : kNumbers)
                            s << predicateValue(predicateSeed, pIdx, timeStamps[i], a, b);
                }
            }
            s << "\n";
        }
        return s.str();
    }
};

// Time stamps are either equidistant (0, 1, 2, ...; as in CommonRoad scenarios) or non-equidistant with
// occasional equal time stamps.
TraceData randomTrace(Generator &g, const size_t maxLength, const bool equidistant) {
    TraceData t;
    const size_t length{g.uniform(1, maxLength)};
    size_t ts{equidistant ? 0 : g.uniform(0, 2)};
    for (size_t i{0}; i < length; ++i) {
        if (i > 0)
            ts += equidistant ? 1 : (g.chance(0.1) ? 0 : g.uniform(1, 3));
        t.timeStamps.push_back(ts);
        std::array<bool, kAtoms.size()> vals{};
        for (auto &v : vals)
            v = g.chance(0.5);
        t.atoms.push_back(vals);
    }
    t.predicateSeed = g.next();
    return t;
}

std::shared_ptr<Trace> buildTrace(const TraceData &t) {
    std::vector<std::vector<std::string>> trueAtoms;
    for (const auto &step : t.atoms) {
        std::vector<std::string> names;
        for (size_t a{0}; a < kAtoms.size(); ++a)
            if (step[a])
                names.emplace_back(kAtoms.at(a));
        trueAtoms.push_back(names);
    }
    const std::set<std::string> allAtoms{kAtoms.begin(), kAtoms.end()};
    const auto events{Utils::createTrace(allAtoms, trueAtoms, t.timeStamps)->getTrace()};

    const uint64_t seed{t.predicateSeed};
    std::vector<std::shared_ptr<IFunction<bool>>> functions;
    for (size_t pIdx{0}; pIdx < kPredicates.size(); ++pIdx) {
        if (kPredicates.at(pIdx).arity == 1)
            functions.push_back(std::make_shared<Function<bool, size_t, size_t>>(
                kPredicates.at(pIdx).name, std::vector<std::string>{"timeStep", "a"},
                [seed, pIdx](const size_t ts, const size_t a) { return predicateValue(seed, pIdx, ts, a, 0); },
                hydraTestUtils::twoSizeTArgs));
        else
            functions.push_back(std::make_shared<Function<bool, size_t, size_t, size_t>>(
                kPredicates.at(pIdx).name, std::vector<std::string>{"timeStep", "a", "b"},
                [seed, pIdx](const size_t ts, const size_t a, const size_t b) {
                    return predicateValue(seed, pIdx, ts, a, b);
                },
                hydraTestUtils::threeSizeTArgs));
    }
    const auto idFunction{[](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }};
    const ConstantCollection constants{
        {ConstantValue{"k0", size_t{0}, idFunction}, ConstantValue{"k1", size_t{1}, idFunction}}};
    return hydraTestUtils::createTraceWithVariable(t.size(), functions, constants, events);
}

std::vector<std::shared_ptr<DomainStorage>> makeDomains() {
    std::vector<std::any> numbers;
    for (const size_t v : kNumbers)
        numbers.emplace_back(v);
    std::vector<std::any> atoms;
    for (const char *a : kAtoms)
        atoms.emplace_back(a);
    return {std::make_shared<DomainStorage>(
                "N", numbers, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }),
            std::make_shared<DomainStorage>(
                "D", atoms, [](const std::any &val) { return std::string(std::any_cast<const char *>(val)); })};
}

// ---------------------------------------------------------------------------------------------------------------------
// Reference semantics (naive, recursive)
// ---------------------------------------------------------------------------------------------------------------------

class Reference {
  public:
    explicit Reference(const TraceData &trace) : t(trace) {}

    bool eval(const NodePtr &n, const size_t i) {
        std::map<std::string, size_t> env;
        return evalEnv(n, i, env);
    }

  private:
    const TraceData &t;

    static size_t atomIndex(const std::string &name) {
        for (size_t a{0}; a < kAtoms.size(); ++a)
            if (name == kAtoms.at(a))
                return a;
        throw std::logic_error("unknown atom " + name);
    }

    static size_t argValue(const std::string &arg, const std::map<std::string, size_t> &env) {
        if (arg == "k0")
            return 0;
        if (arg == "k1")
            return 1;
        return env.at(arg);
    }

    // Evaluates the quantifier body for all variable combinations and returns the number of satisfying ones.
    size_t countSatisfying(const NodePtr &n, const size_t i, std::map<std::string, size_t> &env, size_t varIdx,
                           size_t &total) {
        if (varIdx == n->vars.size()) {
            ++total;
            return evalEnv(n->ch.at(0), i, env) ? 1 : 0;
        }
        const size_t domainSize{n->numDomain ? kNumbers.size() : kAtoms.size()};
        const std::string &var{n->vars.at(varIdx)};
        const auto saved{env.find(var) != env.end() ? std::optional<size_t>(env.at(var)) : std::nullopt};
        size_t count{0};
        for (size_t v{0}; v < domainSize; ++v) {
            env[var] = n->numDomain ? kNumbers.at(v) : v;
            count += countSatisfying(n, i, env, varIdx + 1, total);
        }
        if (saved)
            env[var] = *saved;
        else
            env.erase(var);
        return count;
    }

    bool evalEnv(const NodePtr &n, const size_t i, std::map<std::string, size_t> &env) {
        const size_t len{t.size()};
        switch (n->op) {
        case Op::True:
            return true;
        case Op::False:
            return false;
        case Op::Atom:
            return t.atoms.at(i).at(atomIndex(n->name));
        case Op::VarAtom:
            return t.atoms.at(i).at(env.at(n->name));
        case Op::Pred: {
            const size_t a{argValue(n->args.at(0), env)};
            const size_t b{n->args.size() > 1 ? argValue(n->args.at(1), env) : 0};
            return predicateValue(t.predicateSeed, n->predIdx, t.timeStamps.at(i), a, b);
        }
        case Op::Not:
            return !evalEnv(n->ch.at(0), i, env);
        case Op::And:
            for (const auto &child : n->ch)
                if (!evalEnv(child, i, env))
                    return false;
            return true;
        case Op::Or:
            for (const auto &child : n->ch)
                if (evalEnv(child, i, env))
                    return true;
            return false;
        case Op::Implies:
            return !evalEnv(n->ch.at(0), i, env) or evalEnv(n->ch.at(1), i, env);
        case Op::Next:
            return i + 1 < len and n->iv.contains(t.timeStamps[i + 1] - t.timeStamps[i]) and
                   evalEnv(n->ch.at(0), i + 1, env);
        case Op::Prev:
            return i > 0 and n->iv.contains(t.timeStamps[i] - t.timeStamps[i - 1]) and evalEnv(n->ch.at(0), i - 1, env);
        case Op::Until:
        case Op::Future:
            for (size_t j{i}; j < len; ++j) {
                const bool right{n->op == Op::Future ? evalEnv(n->ch.at(0), j, env) : evalEnv(n->ch.at(1), j, env)};
                if (n->iv.contains(t.timeStamps[j] - t.timeStamps[i]) and right)
                    return true;
                if (n->op == Op::Until and !evalEnv(n->ch.at(0), j, env))
                    return false;
            }
            return false;
        case Op::Globally:
            for (size_t j{i}; j < len; ++j)
                if (n->iv.contains(t.timeStamps[j] - t.timeStamps[i]) and !evalEnv(n->ch.at(0), j, env))
                    return false;
            return true;
        case Op::Since:
        case Op::Once:
            for (size_t j{i + 1}; j-- > 0;) {
                const bool right{n->op == Op::Once ? evalEnv(n->ch.at(0), j, env) : evalEnv(n->ch.at(1), j, env)};
                if (n->iv.contains(t.timeStamps[i] - t.timeStamps[j]) and right)
                    return true;
                if (n->op == Op::Since and !evalEnv(n->ch.at(0), j, env))
                    return false;
            }
            return false;
        case Op::Historically:
            for (size_t j{0}; j <= i; ++j)
                if (n->iv.contains(t.timeStamps[i] - t.timeStamps[j]) and !evalEnv(n->ch.at(0), j, env))
                    return false;
            return true;
        case Op::Exists:
        case Op::Forall:
        case Op::Count:
        case Op::Quote: {
            size_t total{0};
            const size_t count{countSatisfying(n, i, env, 0, total)};
            if (n->op == Op::Exists)
                return count > 0;
            if (n->op == Op::Forall)
                return count == total;
            if (n->op == Op::Count) {
                if (n->countOp == "<=")
                    return count <= n->countNum;
                if (n->countOp == "=")
                    return count == n->countNum;
                return count >= n->countNum;
            }
            // same float arithmetic as the monitor
            const float ratio{static_cast<float>(count) / static_cast<float>(total)};
            return std::stof(quoteText(n->quoteLo)) <= ratio and ratio <= std::stof(quoteText(n->quoteHi));
        }
        }
        return false;
    }
};

// ---------------------------------------------------------------------------------------------------------------------
// Running the monitor
// ---------------------------------------------------------------------------------------------------------------------

struct Config {
    bool sharing;
    bool reordering;
    bool lazy;

    [[nodiscard]] std::string str() const {
        return std::string("sharing=") + (sharing ? "1" : "0") + " reordering=" + (reordering ? "1" : "0") +
               " lazy=" + (lazy ? "1" : "0");
    }

    [[nodiscard]] bool operator==(const Config &o) const {
        return sharing == o.sharing and reordering == o.reordering and lazy == o.lazy;
    }
};

constexpr Config kBaseline{false, false, false};

std::vector<Config> allConfigs() {
    std::vector<Config> cfgs;
    for (const bool s : {false, true})
        for (const bool r : {false, true})
            for (const bool l : {false, true})
                cfgs.push_back({s, r, l});
    return cfgs;
}

enum class Mode { Globally, EarlyTermination, InitialStep };

std::string modeName(const Mode mode) {
    switch (mode) {
    case Mode::Globally:
        return "globally";
    case Mode::EarlyTermination:
        return "early_termination";
    case Mode::InitialStep:
        return "initial_step";
    }
    return "";
}

using PredicateCosts = std::map<std::string, std::array<double, 3>>;

struct Case {
    std::vector<NodePtr> formulas;
    std::vector<TraceData> traces;
    PredicateCosts costs;
};

// verdicts[trace][formula][k]: mode globally -> one verdict per time step; other modes -> a single verdict
using Verdicts = std::vector<std::vector<std::vector<bool>>>;

struct MonitorRun {
    Verdicts verdicts;
    std::string error; // non-empty if the monitor threw
};

// Number of formulas that are monitored in the given mode.
size_t numMonitored(const Case &c, const Mode /*mode*/) { return c.formulas.size(); }

// One monitor instance per call; it is reused for all traces of the case so that resetting is exercised.
MonitorRun runMonitor(const Case &c, const Config &cfg, const Mode mode) {
    MonitorRun run;
    const auto domains{makeDomains()};
    const size_t numFormulas{numMonitored(c, mode)};
    static const std::map<Mode, FormulaEvaluationMode> modes{
        {Mode::Globally, FormulaEvaluationMode::globally},
        {Mode::EarlyTermination, FormulaEvaluationMode::globally_early_termination},
        {Mode::InitialStep, FormulaEvaluationMode::initial_step}};
    try {
        std::vector<Formula> formulas;
        for (size_t k{0}; k < numFormulas; ++k) {
            // initial_step evaluates the formula as is; the other modes assume an implicit top-level globally
            const std::string text{mode == Mode::InitialStep ? toString(c.formulas[k])
                                                             : "G(" + toString(c.formulas[k]) + ")"};
            formulas.push_back(Parser::parse(text, "R" + std::to_string(k)));
        }
        Hydra2 monitor{formulas, cfg.sharing, cfg.reordering, cfg.lazy, modes.at(mode), domains, c.costs};
        for (const auto &t : c.traces) {
            const auto result{monitor.evaluate(buildTrace(t), domains)};
            std::vector<std::vector<bool>> perFormula;
            for (size_t k{0}; k < numFormulas; ++k) {
                std::vector<bool> values;
                for (const auto &v : result.at("R" + std::to_string(k)))
                    values.push_back(v.value);
                perFormula.push_back(values);
            }
            run.verdicts.push_back(perFormula);
        }
    } catch (const std::exception &e) {
        run.error = e.what();
    }
    return run;
}

// Verdicts prescribed by the reference semantics, in the same shape as the monitor output.
Verdicts referenceVerdicts(const Case &c, const Mode mode) {
    Verdicts all;
    for (const auto &t : c.traces) {
        Reference ref{t};
        std::vector<std::vector<bool>> perFormula(numMonitored(c, mode));
        switch (mode) {
        case Mode::Globally:
            for (size_t k{0}; k < perFormula.size(); ++k)
                for (size_t i{0}; i < t.size(); ++i)
                    perFormula[k].push_back(ref.eval(c.formulas[k], i));
            break;
        case Mode::EarlyTermination: {
            // the monitor stops at the first violation (earliest time step, then formula order); only the
            // violated formula reports false
            std::optional<size_t> violated;
            for (size_t i{0}; i < t.size() and !violated; ++i)
                for (size_t k{0}; k < perFormula.size() and !violated; ++k)
                    if (!ref.eval(c.formulas[k], i))
                        violated = k;
            for (size_t k{0}; k < perFormula.size(); ++k)
                perFormula[k] = {!(violated and *violated == k)};
            break;
        }
        case Mode::InitialStep:
            for (size_t k{0}; k < perFormula.size(); ++k)
                perFormula[k] = {ref.eval(c.formulas[k], 0)};
            break;
        }
        all.push_back(perFormula);
    }
    return all;
}

struct Outcome {
    size_t compared{0};
    size_t mismatches{0};
    std::string first; // description of the first mismatch
};

// Compares `actual` with `expected` verdict by verdict; a missing or surplus verdict counts as a mismatch.
Outcome compare(const Verdicts &expected, const Verdicts &actual) {
    Outcome out;
    for (size_t t{0}; t < expected.size(); ++t)
        for (size_t k{0}; k < expected[t].size(); ++k) {
            const auto &exp{expected[t][k]};
            const std::vector<bool> act{t < actual.size() and k < actual[t].size() ? actual[t][k]
                                                                                   : std::vector<bool>{}};
            for (size_t i{0}; i < std::max(exp.size(), act.size()); ++i) {
                ++out.compared;
                if (i < exp.size() and i < act.size() and exp[i] == act[i])
                    continue;
                ++out.mismatches;
                if (out.first.empty()) {
                    std::ostringstream s;
                    s << "trace#" << t << " R" << k << " index " << i << ": expected "
                      << (i < exp.size() ? std::to_string(exp[i]) : "<none>") << ", got "
                      << (i < act.size() ? std::to_string(act[i]) : "<none>");
                    out.first = s.str();
                }
            }
        }
    return out;
}

Outcome compareRuns(const MonitorRun &expected, const MonitorRun &actual) {
    if (expected.error != actual.error) {
        Outcome out;
        out.compared = 1;
        out.mismatches = 1;
        out.first = "exception: expected '" + expected.error + "', got '" + actual.error + "'";
        return out;
    }
    return compare(expected.verdicts, actual.verdicts);
}

// A check maps a case to the comparison outcome; used for counting and for shrinking.
using Check = std::function<Outcome(const Case &)>;

Check againstReference(const Mode mode, const Config cfg) {
    return [mode, cfg](const Case &c) {
        return compareRuns(MonitorRun{referenceVerdicts(c, mode), ""}, runMonitor(c, cfg, mode));
    };
}

Check againstBaseline(const Mode mode, const Config cfg) {
    return [mode, cfg](const Case &c) { return compareRuns(runMonitor(c, kBaseline, mode), runMonitor(c, cfg, mode)); };
}

// ---------------------------------------------------------------------------------------------------------------------
// Shrinking of failing cases
// ---------------------------------------------------------------------------------------------------------------------

// All formulas obtained by replacing one subtree by a simpler, well-scoped alternative.
void shrinkCandidates(const NodePtr &n, const std::set<std::string> &scope, std::vector<NodePtr> &out,
                      const std::function<NodePtr(NodePtr)> &rebuild) {
    const auto wellScoped = [&scope](const NodePtr &cand) {
        for (const auto &v : freeVars(cand))
            if (scope.count(v) == 0)
                return false;
        return true;
    };
    for (const auto &child : n->ch)
        if (wellScoped(child))
            out.push_back(rebuild(child));
    if (n->op != Op::Atom and n->op != Op::True and n->op != Op::False) {
        for (const char *atom : kAtoms) {
            auto leaf{std::make_shared<Node>()};
            leaf->op = Op::Atom;
            leaf->name = atom;
            out.push_back(rebuild(leaf));
        }
    }
    if (n->ch.size() > 2)
        for (size_t i{0}; i < n->ch.size(); ++i) {
            auto fewer{std::make_shared<Node>(*n)};
            fewer->ch.erase(fewer->ch.begin() + static_cast<long>(i));
            out.push_back(rebuild(fewer));
        }
    if (n->iv.printed and n->iv.lo > 0) {
        auto simpler{std::make_shared<Node>(*n)};
        simpler->iv.lo = 0;
        out.push_back(rebuild(simpler));
    }
    std::set<std::string> innerScope{scope};
    innerScope.insert(n->vars.begin(), n->vars.end());
    for (size_t i{0}; i < n->ch.size(); ++i) {
        shrinkCandidates(n->ch[i], innerScope, out, [&n, &rebuild, i](NodePtr repl) {
            auto copy{std::make_shared<Node>(*n)};
            copy->ch[i] = std::move(repl);
            return rebuild(copy);
        });
    }
}

// Greedy shrinking: repeatedly applies the first simplification that keeps the case failing.
Case shrink(Case c, const Check &check) {
    const auto tryCandidate = [&c, &check](const Case &cand) {
        for (const auto &f : cand.formulas)
            if (!quantifiedVariablesUsed(f))
                return false;
        if (check(cand).mismatches == 0)
            return false;
        c = cand;
        return true;
    };
    bool progress{true};
    while (progress) {
        progress = false;
        for (size_t k{0}; k < c.formulas.size() and c.formulas.size() > 1 and !progress; ++k) {
            Case cand{c};
            cand.formulas.erase(cand.formulas.begin() + static_cast<long>(k));
            progress = tryCandidate(cand);
        }
        for (size_t tIdx{0}; tIdx < c.traces.size() and c.traces.size() > 1 and !progress; ++tIdx) {
            Case cand{c};
            cand.traces.erase(cand.traces.begin() + static_cast<long>(tIdx));
            progress = tryCandidate(cand);
        }
        for (size_t tIdx{0}; tIdx < c.traces.size() and !progress; ++tIdx)
            for (const bool dropFirst : {false, true}) {
                if (c.traces[tIdx].size() <= 1 or progress)
                    continue;
                Case cand{c};
                auto &t{cand.traces[tIdx]};
                const auto pos{dropFirst ? 0L : static_cast<long>(t.size() - 1)};
                t.timeStamps.erase(t.timeStamps.begin() + pos);
                t.atoms.erase(t.atoms.begin() + pos);
                progress = tryCandidate(cand);
            }
        for (size_t k{0}; k < c.formulas.size() and !progress; ++k) {
            std::vector<NodePtr> candidates;
            shrinkCandidates(c.formulas[k], {}, candidates, [](NodePtr r) { return r; });
            for (const auto &f : candidates) {
                if (nodeCount(f) > nodeCount(c.formulas[k]) or toString(f) == toString(c.formulas[k]))
                    continue;
                Case cand{c};
                cand.formulas[k] = f;
                if ((progress = tryCandidate(cand)))
                    break;
            }
        }
        for (size_t tIdx{0}; tIdx < c.traces.size() and !progress; ++tIdx)
            for (size_t i{0}; i < c.traces[tIdx].size() and !progress; ++i)
                for (size_t a{0}; a < kAtoms.size() and !progress; ++a) {
                    if (!c.traces[tIdx].atoms[i][a])
                        continue;
                    Case cand{c};
                    cand.traces[tIdx].atoms[i][a] = false;
                    progress = tryCandidate(cand);
                }
        if (!progress and !c.costs.empty()) {
            Case cand{c};
            cand.costs.clear();
            progress = tryCandidate(cand);
        }
    }
    return c;
}

std::string describe(const Case &c, const Check &check, const Mode mode) {
    std::ostringstream s;
    for (size_t k{0}; k < numMonitored(c, mode); ++k)
        s << "  R" << k << ": "
          << (mode == Mode::InitialStep ? toString(c.formulas[k]) : "G(" + toString(c.formulas[k]) + ")") << "\n";
    for (const auto &[name, cost] : c.costs)
        s << "  predicate cost " << name << " = {" << cost[0] << ", " << cost[1] << ", " << cost[2] << "}\n";
    for (size_t tIdx{0}; tIdx < c.traces.size(); ++tIdx)
        s << "  trace#" << tIdx << ":\n" << c.traces[tIdx].str();
    s << "  first mismatch: " << check(c).first << "\n";
    return s.str();
}

// ---------------------------------------------------------------------------------------------------------------------
// Test driver
// ---------------------------------------------------------------------------------------------------------------------

Case randomCase(Generator &g, const size_t maxFormulas, const bool equidistant) {
    constexpr size_t kMaxDepth{4};
    constexpr size_t kTracesPerCase{3};
    constexpr size_t kMaxTraceLength{10};
    Case c;
    const size_t numFormulas{g.uniform(1, maxFormulas)};
    for (size_t k{0}; k < numFormulas; ++k)
        c.formulas.push_back(g.formula(g.uniform(1, kMaxDepth)));
    for (size_t tIdx{0}; tIdx < kTracesPerCase; ++tIdx)
        c.traces.push_back(randomTrace(g, kMaxTraceLength, equidistant));
    static const std::vector<double> priorities{0.0, 0.0, 1.0};
    for (const auto &pred : kPredicates)
        if (g.chance(0.8))
            c.costs[pred.name] = {static_cast<double>(g.uniform(1, 100)), static_cast<double>(g.uniform(5, 95)) / 100.0,
                                  g.pick(priorities)};
    return c;
}

struct Totals {
    size_t compared{0};
    size_t mismatches{0};
    size_t failingCases{0};
};

struct CampaignResult {
    size_t cases{0};
    size_t formulas{0};
    size_t traces{0};
    Totals equivalence;                                // each optimized configuration vs. the unoptimized baseline
    Totals reference;                                  // each configuration vs. the reference semantics
    std::map<std::string, size_t> mismatchesPerConfig; // optimized vs. baseline
};

// Number of shrunk counterexamples printed per category (environment variable RVM_DIFF_REPORT, default 3).
size_t reportLimit() {
    const char *env{std::getenv("RVM_DIFF_REPORT")};
    return env == nullptr ? 3 : static_cast<size_t>(std::stoul(env));
}

CampaignResult runCampaign(const uint64_t seed, const size_t numCases, const size_t maxFormulas, const Mode mode,
                           const bool equidistant, const std::vector<Config> &configs) {
    Generator g{seed};
    CampaignResult res;
    std::map<std::string, std::set<std::string>> reported; // category -> shrunk counterexamples
    const size_t limit{reportLimit()};
    const auto report = [&](const std::string &category, const Case &c, const Check &check, const Config &cfg) {
        if (reported[category].size() >= limit)
            return;
        const std::string text{describe(shrink(c, check), check, mode)};
        if (reported[category].insert(text).second)
            ADD_FAILURE() << "[" << modeName(mode) << "] " << category << " (" << cfg.str() << ")\n" << text;
    };

    for (size_t caseIdx{0}; caseIdx < numCases; ++caseIdx) {
        const Case c{randomCase(g, maxFormulas, equidistant)};
        ++res.cases;
        res.formulas += numMonitored(c, mode);
        res.traces += c.traces.size();
        const MonitorRun expected{referenceVerdicts(c, mode), ""};
        const MonitorRun baseline{runMonitor(c, kBaseline, mode)};
        bool equivalenceFailed{false};
        bool referenceFailed{false};
        for (const auto &cfg : configs) {
            const MonitorRun run{cfg == kBaseline ? baseline : runMonitor(c, cfg, mode)};
            const Outcome ref{compareRuns(expected, run)};
            res.reference.compared += ref.compared;
            res.reference.mismatches += ref.mismatches;
            if (ref.mismatches > 0) {
                referenceFailed = true;
                report("disagreement with reference semantics", c, againstReference(mode, cfg), cfg);
            }
            if (cfg == kBaseline)
                continue;
            const Outcome eq{compareRuns(baseline, run)};
            res.equivalence.compared += eq.compared;
            res.equivalence.mismatches += eq.mismatches;
            res.mismatchesPerConfig[cfg.str()] += eq.mismatches;
            if (eq.mismatches > 0) {
                equivalenceFailed = true;
                report("optimization changes verdicts", c, againstBaseline(mode, cfg), cfg);
            }
        }
        res.equivalence.failingCases += equivalenceFailed ? 1 : 0;
        res.reference.failingCases += referenceFailed ? 1 : 0;
    }
    std::cout << "[differential " << modeName(mode) << (equidistant ? ", equidistant" : ", non-equidistant")
              << "] cases=" << res.cases << " formulas=" << res.formulas << " traces=" << res.traces
              << " configurations=" << configs.size() << "\n"
              << "  optimized vs. baseline: verdicts=" << res.equivalence.compared
              << " mismatches=" << res.equivalence.mismatches << " failing_cases=" << res.equivalence.failingCases
              << "\n"
              << "  monitor vs. reference:  verdicts=" << res.reference.compared
              << " mismatches=" << res.reference.mismatches << " failing_cases=" << res.reference.failingCases << "\n";
    for (const auto &[cfg, count] : res.mismatchesPerConfig)
        std::cout << "  optimized vs. baseline [" << cfg << "]: mismatches=" << count << "\n";
    std::cout << std::flush;
    return res;
}

} // namespace

// Mode `globally` (verdict at every time step) with up to three formulas monitored jointly, so that subformulas are
// shared across roots. All eight combinations of sharing, reordering, and lazy evaluation are compared with the
// unoptimized baseline and with the reference semantics.
TEST(TestHydra2Differential, globallyEquidistant) {
    const auto res{runCampaign(20260920ULL, 1000, 3, Mode::Globally, true, allConfigs())};
    EXPECT_EQ(res.equivalence.mismatches, 0U) << "optimizations changed verdicts";
    EXPECT_EQ(res.reference.mismatches, 0U) << "monitor disagrees with the reference semantics";
}

TEST(TestHydra2Differential, globallyNonEquidistant) {
    const auto res{runCampaign(20260921ULL, 1000, 3, Mode::Globally, false, allConfigs())};
    EXPECT_EQ(res.equivalence.mismatches, 0U) << "optimizations changed verdicts";
    EXPECT_EQ(res.reference.mismatches, 0U) << "monitor disagrees with the reference semantics";
}

TEST(TestHydra2Differential, earlyTermination) {
    const auto res{runCampaign(20260922ULL, 500, 3, Mode::EarlyTermination, true, allConfigs())};
    EXPECT_EQ(res.equivalence.mismatches, 0U) << "optimizations changed verdicts";
    EXPECT_EQ(res.reference.mismatches, 0U) << "monitor disagrees with the reference semantics";
}

TEST(TestHydra2Differential, initialStep) {
    const auto res{runCampaign(20260923ULL, 500, 3, Mode::InitialStep, true, allConfigs())};
    EXPECT_EQ(res.equivalence.mismatches, 0U) << "optimizations changed verdicts";
    EXPECT_EQ(res.reference.mismatches, 0U) << "monitor disagrees with the reference semantics";
}
