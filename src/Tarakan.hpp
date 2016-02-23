#ifndef TARAKAN_HPP
#define TARAKAN_HPP
#include <string>

#include "ISerializable.hpp"

class Tarakan : public ISerializable<Tarakan> {
	public:
		std::string id;
};
#endif //TARAKAN_HPP

