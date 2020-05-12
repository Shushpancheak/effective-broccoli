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

inline ValueStorage<std::function<bool   (void)>> coin_rand;
inline ValueStorage<std::function<double (void)>> double_rand;
inline ValueStorage<std::function<float  (void)>> float_rand;

}


inline std::mt19937 GetDevice() {
  return *detail::random_device.PtrUnsafe();
}

inline bool GetCoinRand() {
  return (*detail::coin_rand.PtrUnsafe())();
}

inline double GetDoubleRand() {
  return (*detail::double_rand.PtrUnsafe())();
}

inline double GetFloatRand() {
  return (*detail::float_rand.PtrUnsafe())();
}


inline Status Init(size_t seed = Clock::now().time_since_epoch().count()) {
  detail::random_device.Construct(seed);

  detail::coin_rand.Construct(
    std::bind(std::uniform_int_distribution<int>(false, true),
              GetDevice())
  );
  detail::double_rand.Construct(
    std::bind(std::uniform_real_distribution<double>(0, 1),
              GetDevice())
  );
  detail::float_rand.Construct(
    std::bind(std::uniform_real_distribution<float>(0, 1),
              GetDevice())
  );

  return make_result::Ok();
}

inline Status Terminate() {
  detail::float_rand.Destroy();
  detail::double_rand.Destroy();
  detail::coin_rand.Destroy();

  detail::random_device.Destroy();

  return make_result::Ok();
}

inline bool CoinFlip(const double p) {
  return GetDoubleRand() < p;
}

inline bool CoinFlipFloat(const float p) {
  return GetFloatRand() < p;
}

inline sf::Vector2f GetVector2fRand() {
  return sf::Vector2f(rnd::GetFloatRand(), rnd::GetFloatRand());
}

inline sf::Vector2f GetVector2fNormalRand() {
  return sf::Vector2f(rnd::GetFloatRand() - 0.5f, rnd::GetFloatRand() - 0.5f);
}

}

#endif // EFFECTIVE_BROCCOLI_RANDOM_HPP
