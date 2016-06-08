#ifndef RANDOM_STRATEGY_HPP
#define RANDOM_STRATEGY_HPP

#include "IStrategy.hpp"
#include "ISerializable.hpp"

#include <functional>


class RandomStrategy : public IStrategy {
  public:
    RandomStrategy();
    ~RandomStrategy();

    unsigned int exec();

  private:
    std::function<unsigned int()> uint_dist;
};


#endif // RANDOM_STRATEGY_HPP
