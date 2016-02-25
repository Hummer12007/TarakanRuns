#ifndef ISERIALIZABLE_HPP
#define ISERIALIZABLE_HPP

#include <string>

template <typename T> class ISerializable {
  public:
    virtual ~ISerializable(void) {}
    virtual std::string serialize(void) = 0;
    virtual void deserialize(std::string &data) = 0;
};

#endif // ISERIALIZABLE_HPP
