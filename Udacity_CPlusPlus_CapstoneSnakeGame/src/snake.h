#ifndef SNAKE_H
#define SNAKE_H

#include "GameObject.h"
#include "SDL.h"
#include <vector>

class Snake : public GameObject {
public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height) : GameObject(grid_width, grid_height) {
    head_x = static_cast<int>(grid_width / 2);
    head_y = static_cast<int>(grid_height / 2);
  }

  // Destructor
  ~Snake() = default;

  // Copy constructor
  Snake(const Snake &other)
      : GameObject(other), speed(other.speed), size(other.size),
        growing(other.growing), body(other.body) {}

  // Copy assignment operator
  Snake &operator=(const Snake &other) {
    if (this == &other)
      return *this;               // Self-assignment check
    GameObject::operator=(other); // Copy base class properties
    speed = other.speed;
    size = other.size;
    growing = other.growing;
    body = other.body;
    return *this;
  }

  // Move constructor
  Snake(Snake &&other) noexcept
      : GameObject(std::move(other)), speed(other.speed), size(other.size),
        growing(other.growing), body(std::move(other.body)) {
    // Reset moved-from object state if needed
    other.speed = 0.0f;
    other.size = 0;
    other.growing = false;
  }

  // Move assignment operator
  Snake &operator=(Snake &&other) noexcept {
    if (this == &other)
      return *this;                          // Self-assignment check
    GameObject::operator=(std::move(other)); // Move base class properties
    speed = other.speed;
    size = other.size;
    growing = other.growing;
    body = std::move(other.body);

    // Reset moved-from object state if needed
    other.speed = 0.0f;
    other.size = 0;
    other.growing = false;
    return *this;
  }

  void Update();
  void GrowBody();
  bool ObjectCell(int x, int y) override;
  std::vector<SDL_Point> getBody() const;

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};

private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  std::vector<SDL_Point> body;
};

#endif