#ifndef ISTRATEGY_HPP
#define ISTRATEGY_HPP


class IStrategy {
  public:
    virtual ~IStrategy() = 0;
    virtual unsigned int exec() = 0;
};

inline IStrategy::~IStrategy() {}


#endif // !ISTRATEGY_HPP
