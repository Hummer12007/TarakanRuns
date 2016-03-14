#ifndef IMESSAGEABLE_HPP
#define IMESSAGEABLE_HPP

#include <string>
#include <functional>

class IMessageable {
  public:
    virtual ~IMessageable(void) = 0;
    virtual void receive_message(std::string, std::function<void(string)>) = 0;
};

inline ISerializable<T>::~ISerializable(void) {}

#endif // IMESSAGEABLE_HPP
