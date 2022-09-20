#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <monitors/formula/formula.h>
#include <monitors/parser/parser.h>
#include <monitors/rv_algorithms/hydra_2/hydra.h>

static void print_solution(const std::vector<Verdict> &solution) {
    int i = -1;
    int last_ts = -1;
    std::cout << std::boolalpha;
    for (const auto &verdict : solution) {
        assert(verdict.timeStamp.has_value());
        auto ts = verdict.timeStamp.value();
        if (ts == last_ts) {
            i++;
        } else {
            i = 0;
        }
        std::cout << ts << ":" << i << " " << verdict.value << std::endl;
        last_ts = ts;
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " FORMULA LOG" << std::endl;
        return EXIT_FAILURE;
    }

    std::string formula_path{argv[1]};
    std::string log_path{argv[2]};

    std::ifstream formula_file(formula_path);
    std::stringstream formula_buffer;
    formula_buffer << formula_file.rdbuf();

    std::ifstream log_file(log_path);
    std::stringstream log_buffer;
    log_buffer << log_file.rdbuf();

    Formula formula = Parser::parse(formula_buffer.str(), "Ex1");

    Trace trace = Parser::parse_log(log_buffer.str());

    auto monitor{Hydra2({formula})};
    auto solution_map{monitor.evaluate(trace)};
    const auto &solution = solution_map[formula.getId()];
    print_solution(solution);

    return EXIT_SUCCESS;
}