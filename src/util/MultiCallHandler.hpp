#ifndef MULTI_CALL_HANDLER_HPP
#define MULTI_CALL_HANDLER_HPP

#include "util/logging.hpp"

#include <string>
#include <map>
#include <functional>


class MultiCallHandler {
  public:
    MultiCallHandler(std::string const& program_name);

    int exec(int argc, char **argv) const;

    void addFunction(std::string const& function_name,
                     std::function<int(int, char **)> function);

  private:
    int checkCalledProgram(int argc, char **argv) const;
    int printUsage(bool on_failure) const;

    std::string program_name;
    std::map<std::string, std::function<int(int, char **)>> functions;
    Logger logger;
};


#endif // MULTI_CALL_HANDLER_HPP
