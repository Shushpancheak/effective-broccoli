#ifndef EFFECTIVE_BROCOLLI_TYPE_COUNTER_HPP
#define EFFECTIVE_BROCOLLI_TYPE_COUNTER_HPP

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

private:
  static size_t cnt_;
};

#endif //EFFECTIVE_BROCOLLI_TYPE_COUNTER_HPP
