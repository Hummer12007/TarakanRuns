#ifndef IMESSAGEABLE_HPP
#define IMESSAGEABLE_HPP

#include <string>
#include <functional>

class IMessageable {
  private:
    typedef std::function<void(std::string const &)> Callback;

  public:
    virtual ~IMessageable(void) = 0;
    virtual void receive_message(std::string const &message,
                                 IMessageable::Callback &fun) = 0;
};

// Even pure virtual destructor should have an implementation
inline IMessageable::~IMessageable(void) {}

#endif // IMESSAGEABLE_HPP
