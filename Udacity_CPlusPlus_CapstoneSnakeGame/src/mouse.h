#ifndef MOUSE_H
#define MOUSE_H

#include "SDL.h"
#include <random>
#include <vector>

class Mouse {
public:
  Mouse(int grid_width, int grid_height)
      : grid_width(grid_width), grid_height(grid_height) {
    NewMouse();
  }

  void UpdatePosition(int x, int y);
  bool MouseCell(int x, int y);
  void NewMouse();

  int randomPosition(int lower, int upper);
  bool alive{true};
  int positionX;
  int positionY;

private:
  int grid_width;
  int grid_height;
};

#endif