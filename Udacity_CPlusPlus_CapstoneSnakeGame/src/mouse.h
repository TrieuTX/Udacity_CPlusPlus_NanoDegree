#ifndef MOUSE_H
#define MOUSE_H

#include "GameObject.h"
#include "SDL.h"
#include <random>
#include <vector>

class Mouse : public GameObject {
public:
  Mouse(int grid_width, int grid_height) : GameObject(grid_width, grid_height) {
    NewMouse();
  }

  // Destructor
  ~Mouse() = default;

  // Copy constructor
  Mouse(const Mouse &other) : GameObject(other) {
    // Additional properties if any
  }

  // Copy assignment operator
  Mouse &operator=(const Mouse &other) {
    if (this == &other)
      return *this;               // Self-assignment check
    GameObject::operator=(other); // Copy base class properties
    return *this;
  }

  // Move constructor
  Mouse(Mouse &&other) noexcept : GameObject(std::move(other)) {}

  // Move assignment operator
  Mouse &operator=(Mouse &&other) noexcept {
    if (this == &other)
      return *this;                          // Self-assignment check
    GameObject::operator=(std::move(other)); // Move base class properties;
    return *this;
  }

  void UpdatePosition(int x, int y);
  bool ObjectCell(int x, int y) override;
  void NewMouse();

  int randomPosition(int lower, int upper);
};

#endif