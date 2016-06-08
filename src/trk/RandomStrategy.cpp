#include "RandomStrategy.hpp"

#include <random>
#include <functional>

#include <json.hpp>

using json = nlohmann::json;


RandomStrategy::RandomStrategy() {
    std::random_device rdev;
    std::default_random_engine engine(rdev());
    std::uniform_int_distribution<unsigned int> uniform_dist(1, 5);

    this->uint_dist = std::bind(uniform_dist, engine);
}

RandomStrategy::~RandomStrategy() {}


unsigned int RandomStrategy::exec() {
    return uint_dist();
}
