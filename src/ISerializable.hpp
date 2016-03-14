#ifndef ISERIALIZABLE_HPP
#define ISERIALIZABLE_HPP

#include <string>

class ISerializable {
  public:
    virtual ~ISerializable(void) = 0;
    virtual std::string serialize(void) = 0;
    virtual void deserialize(std::string const &data) = 0;
};

// Even pure virtual destructor should have an implementation
inline ISerializable::~ISerializable(void) {}

#endif // ISERIALIZABLE_HPP
