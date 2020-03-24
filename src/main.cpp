#include "Entity.hpp"
#include "ResourceManager.hpp"
#include "SFML/Graphics.hpp"
#include "Component.hpp"

#include <iostream>

class TestComponent1 : public Component<TestComponent1> {

};

class TestComponent2 : public Component<TestComponent2> {

};

int main() {

  return 0;
}