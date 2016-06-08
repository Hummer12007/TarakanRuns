#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>


bool startsWith(std::string const& subject, std::string const& prefix);
bool endsWith(std::string const& subject, std::string const& suffix);

std::string escape(std::string const& s);
std::string unescape(std::string const& s);


#endif // STRINGUTILS_HPP
