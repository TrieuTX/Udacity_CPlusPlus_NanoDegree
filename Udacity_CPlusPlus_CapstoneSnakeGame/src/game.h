#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "controller.h"
#include "mouse.h"
#include "renderer.h"
#include "snake.h"
#include <algorithm>
#include <map>
#include <math.h>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <thread>

struct Node {
  int x, y;
  int f, g, h;
  bool operator<(const Node &other) const { return f > other.f; }
};
class Game {
public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

private:
  std::unique_ptr<Snake> snake;
  std::unique_ptr<Mouse> mouse;
  std::vector<SDL_Point> mouseASearchPath{};
  SDL_Point food;

  // for wall
  std::size_t kGridWidth;
  std::size_t kGridHeight;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void NewMouse();
  void Update();

  // A* Algorithm functions
  std::vector<SDL_Point> AStarMouse(const SDL_Point &start,
                                    const SDL_Point &goal);
  int heuristic(const SDL_Point &a, const SDL_Point &b);

  // Thread and mutex for auto-running mouse
  std::thread auto_run_mouse;
  std::mutex mouse_mutex;
  bool auto_running_mouse{true};
  void AutoRunMouse();
};

#endif