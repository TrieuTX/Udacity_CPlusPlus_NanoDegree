#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SDL.h"
#include <vector>

class GameObject {
public:
  GameObject(int grid_width, int grid_height)
      : grid_width(grid_width), grid_height(grid_height) {}

  virtual ~GameObject() = default;

  // Copy constructor
  GameObject(const GameObject &other)
      : grid_width(other.grid_width), grid_height(other.grid_height),
        alive(other.alive), head_x(other.head_x), head_y(other.head_y) {}

  // Copy assignment operator
  GameObject &operator=(const GameObject &other) {
    if (this == &other)
      return *this; // Self-assignment check
    grid_width = other.grid_width;
    grid_height = other.grid_height;
    alive = other.alive;
    head_x = other.head_x;
    head_y = other.head_y;
    return *this;
  }

  // Move constructor
  GameObject(GameObject &&other) noexcept
      : grid_width(other.grid_width), grid_height(other.grid_height),
        alive(other.alive), head_x(other.head_x), head_y(other.head_y) {
    // Optionally, reset the state of the moved-from object if necessary
    other.grid_width = 0;
    other.grid_height = 0;
    other.alive = false;
    other.head_x = 0;
    other.head_y = 0;
  }

  // Move assignment operator
  GameObject &operator=(GameObject &&other) noexcept {
    if (this == &other)
      return *this; // Self-assignment check
    grid_width = other.grid_width;
    grid_height = other.grid_height;
    alive = other.alive;
    head_x = other.head_x;
    head_y = other.head_y;

    // Reset the state of the moved-from object
    other.grid_width = 0;
    other.grid_height = 0;
    other.alive = false;
    other.head_x = 0;
    other.head_y = 0;

    return *this;
  }

  virtual bool ObjectCell(int x, int y) = 0;

  // Accessors
  bool IsAlive() const { return alive; }
  int GetHeadX() const { return head_x; }
  int GetHeadY() const { return head_y; }
  void SetHead(int x, int y) {
    head_x = x;
    head_y = y;
  }
  void setAlive(bool input) { alive = input; }

protected:
  bool alive{true};
  float head_x;
  float head_y;
  int grid_width;
  int grid_height;
};

#endif