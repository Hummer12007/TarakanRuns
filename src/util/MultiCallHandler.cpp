#include "MultiCallHandler.hpp"

#include "util/stringutils.hpp"

#include <string>
#include <functional>
#include <iostream>


MultiCallHandler::MultiCallHandler(std::string const& program_name)
        : program_name(program_name),
          functions(),
          logger("MultiCallHandler") {}


int MultiCallHandler::exec(int argc, char **argv) const {
    this->logger.debug("argc: ", argc);
    this->logger.debug("argv[0]: \"", argv[0], "\"");

    if (endsWith(argv[0], this->program_name)) {
        return (argc == 1)
               ? this->printUsage(true)
               : this->checkCalledProgram(argc - 1, argv + 1);
    }

    return this->checkCalledProgram(argc, argv);
}

void MultiCallHandler::addFunction(
        std::string const& function_name,
        std::function<int(int, char **)> function) {
    this->functions[function_name] = function;
}


int MultiCallHandler::checkCalledProgram(int argc, char **argv) const {
    for (auto const& kv : this->functions) {
        if (endsWith(argv[0], kv.first)) {
            return kv.second(argc, argv);
        }
    }

    return printUsage(true);
}

int MultiCallHandler::printUsage(bool on_failure) const {
    std::ostream& os = (on_failure) ? std::cerr : std::cout;

    // TODO: print usage message
    os << "Usage: "
           << this->program_name
           << " [function [arguments]...]" << std::endl
       << "   or: function [arguments]..." << std::endl;

    return (int) on_failure;
}
