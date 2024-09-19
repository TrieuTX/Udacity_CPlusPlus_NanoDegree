#include "mouse.h"
#include <cmath>
#include <iostream>

bool Mouse::MouseCell(int x, int y) {
  if (x == static_cast<int>(positionX) && y == static_cast<int>(positionY)) {
    return true;
  }
  return false;
}

void Mouse::UpdatePosition(int x, int y) {
  positionX = x;
  positionY = y;
}

int Mouse::randomPosition(int lower, int upper) {

  // Create a random number generator
  std::random_device rd;  // Seed for the random number engine
  std::mt19937 gen(rd()); // Mersenne Twister engine
  std::uniform_int_distribution<> distr(lower, upper); // Define range

  // Generate random number
  int random_number = distr(gen);
  std::cout << "Random: " << random_number << std::endl;
  return random_number;
}

void Mouse::NewMouse() {
  UpdatePosition(randomPosition(1, static_cast<int>(grid_width - 2)),
                 randomPosition(1, static_cast<int>(grid_height - 2)));
  alive = true;
  std::cout << "New Mouse: " << positionX << " " << positionY << std::endl;
}