#ifndef EFFECTIVE_BROCOLLI_CIRCLE
#define EFFECTIVE_BROCOLLI_CIRCLE

#include <iostream>
#include "Entity.hpp"

class Circle : public Entity<Circle> {
public:
  Circle();

private:
  static size_t obj_cnt_;
};

inline Circle::Circle() : Entity<Circle>(0) {
  std::cout << "I'm circle that has TypeID == " << Circle::GetTypeID() << ", and I'm number " << obj_cnt_++ << "! Also, I have EntityID == " << GetEntityID << "." << std::endl;
}

#endif
