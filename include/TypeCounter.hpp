#ifndef EFFECTIVE_BROCOLLI_TYPECOUNTER_HPP
#define EFFECTIVE_BROCOLLI_TYPECOUNTER_HPP

#include <cstddef>


class TypeCounter {
public:
  template <typename U>
  static size_t GetId() {
     return cnt_++;
  }

  static size_t GetId() {
    return cnt_;
  }

  static size_t cnt_;
};

#endif //EFFECTIVE_BROCOLLI_TYPECOUNTER_HPP
