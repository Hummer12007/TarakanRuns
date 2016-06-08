#include "stringutils.hpp"

#include <string>
#include <sstream>
#include <ios>
#include <iomanip>
#include <stdexcept>

#include <cctype>


bool startsWith(std::string const& subject, std::string const& prefix) {
    return (subject.compare(0, prefix.size(), prefix) == 0);
}

bool endsWith(std::string const& subject, std::string const& suffix) {
    return (subject.size() >= suffix.size()) &&
           (subject.compare(subject.size() - suffix.size(), subject.size(),
                            suffix, 0, suffix.size()) == 0);
}


static void escapeChar(std::ostringstream& oss, char c) {
    switch (c) {
        case '\'': oss << "\\\'"; break;
        case '\"': oss << "\\\""; break;
        case '\?': oss << "\\\?"; break;
        case '\\': oss << "\\\\"; break;
        case '\a': oss << "\\a";  break;
        case '\b': oss << "\\b";  break;
        case '\f': oss << "\\f";  break;
        case '\n': oss << "\\n";  break;
        case '\r': oss << "\\r";  break;
        case '\t': oss << "\\t";  break;
        case '\v': oss << "\\v";  break;
        default:
            if (std::isgraph(c)) {    // we need space \x-escaped
                oss << c;
            } else {
                oss << "\\x"
                    << std::hex << std::setfill('0') << std::setw(2)
                    << (int) c;
            }
    }
}

static std::string escapeChar(char c) {
    std::ostringstream oss;

    escapeChar(oss, c);

    return oss.str();
}

std::string escape(std::string const& s) {
    std::ostringstream oss;
    oss << "\"";

    for (char c : s) {
        escapeChar(oss, c);
    }

    oss << "\"";

    return oss.str();
}


static char hexToChar(std::string const& hex) {
    if (hex.size() != 2) {
        throw std::runtime_error(
                "Wrong hex length: \"" + hex + "\" "
                "(len = " + std::to_string(hex.size()) + ")");
    } else if (!isxdigit(hex.at(0)) || !isxdigit(hex.at(1))) {
        throw std::runtime_error("Wrong hex value: \"" + hex + "\"");
    }

    char result = 0;

    for (size_t i = 0; i < hex.size(); ++i) {
        char c = hex.at(i);

        int hexValue = (c >= '0' && c <= '9') ? c - '0' :
                       (c >= 'a' && c <= 'f') ? c - 'a' + 10 :
                       (c >= 'A' && c <= 'F') ? c - 'A' + 10 : -1;
        result += i == 0 ? hexValue << 4 : hexValue;
    }

    return result;
}

std::string unescape(std::string const& s) {
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s.at(i);

        if (!isprint(c)) {
            throw std::invalid_argument(
                    "Malformed escaped string. "
                    "Unprintable character at position " + std::to_string(i) +
                    ": \'" + escapeChar(c) + "\'");
        }
    }

    if (s.at(0) != '\"' || s.at(s.size() - 1) != '\"') {
        throw std::invalid_argument(
                "Malformed escaped string. "
                "The first and the last symbol must be \\\"" + s);
    }


    std::ostringstream oss;

    for (size_t i = 0; i < s.size(); ++i) {
        if (i == 0 || i == s.size() - 1) {
            continue;
        }

        char c = s.at(i);

        if (c == '\\') {
            if (i + 1 == s.size() - 1) {
                throw std::invalid_argument(
                        "Malformed escaped string. "
                        "Escaping nothing at the end");
            }

            char next = s.at(i + 1);

            switch (next) {
                case '\'':
                case '\"':
                case '\?':
                case '\\':
                    oss << next;
                    break;
                case 'a': oss << '\a'; break;
                case 'b': oss << '\b'; break;
                case 'f': oss << '\f'; break;
                case 'n': oss << '\n'; break;
                case 'r': oss << '\r'; break;
                case 't': oss << '\t'; break;
                case 'v': oss << '\v'; break;
                case 'x':
                    if (i + 2 == s.size() - 1 || i + 3 == s.size() - 1) {
                        throw std::invalid_argument(
                                "Malformed escaped string. "
                                "Incomplete hex value at the end");
                    }

                    oss << hexToChar({s.at(i + 2), s.at(i + 3)});
                    i += 2;
                    break;
                default:
                    throw std::invalid_argument(
                            "Malformed escaped string. "
                            "Not an escape character at position " +
                            std::to_string(i) +
                            ": \"" + "\\" + std::string(1, next) + "\"");
            }

            ++i;
        } else {
            oss << c;
        }
    }

    return oss.str();
}
