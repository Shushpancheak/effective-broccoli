#ifndef BROCOLLI_ENTITY
#define BROCOLLI_ENTITY

class Entity {
public:
  Entity();

private:
  size_t id_;
  static size_t current_id_;
};

#endif // BROCOLLI_ENTITY
