#ifndef EFFECTIVE_BROCCOLI_RANDOM_HPP
#define EFFECTIVE_BROCCOLI_RANDOM_HPP

#include <functional>
#include <random>

#include "result.hpp"
#include "value_storage.hpp"
#include "time.hpp"

namespace rnd {

namespace detail {

inline ValueStorage<std::mt19937> random_device;

inline ValueStorage<decltype(
    std::bind(std::uniform_int_distribution<bool>(false, true), std::mt19937(time(nullptr)))
  )> coin_rand;

inline ValueStorage<decltype(
    std::bind(std::uniform_real_distribution<double>(0, 1), std::mt19937(time(nullptr)))
  )> real_rand;

}


inline std::mt19937* GetDevice() {
  return detail::random_device.PtrUnsafe();
}


inline Status Init(size_t seed = Clock::now().time_since_epoch().count()) {
  detail::random_device.Construct(seed);

  detail::coin_rand.Construct(
    std::bind(std::uniform_int_distribution<bool>(false, true),
              GetDevice)
  );

  detail::real_rand.Construct(
    std::bind(std::uniform_real_distribution<double>(0, 1),
              GetDevice)
  );

  return make_result::Ok();
}

inline Status Terminate() {
  detail::random_device.Destroy();

  return make_result::Ok();
}

inline bool CoinFlip(double p) {
  return GetDevice().
}

}

#endif // EFFECTIVE_BROCCOLI_RANDOM_HPP
