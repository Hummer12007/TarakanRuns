#ifndef ISERIALIZABLE_HPP
#define ISERIALIZABLE_HPP
#include <string>

template <typename T>
class ISerializable {
	public:
		virtual ~ISerializable(){}
		virtual T deserialize(std::string) /* = 0*/ {};
		virtual std::string serialize(T) /* = 0*/ {};
};
#endif //ISERIALIZABLE_HPP

