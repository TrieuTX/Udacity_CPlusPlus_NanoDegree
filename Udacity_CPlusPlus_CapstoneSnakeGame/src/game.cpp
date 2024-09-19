#include "game.h"
#include "SDL.h"
#include <iostream>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height), mouse(grid_width, grid_height),
      engine(dev()), random_w(1, static_cast<int>(grid_width - 2)),
      random_h(1, static_cast<int>(grid_height - 2)), kGridWidth(grid_width),
      kGridHeight(grid_height) {
  PlaceFood();
  auto_run_mouse = std::thread(&Game::AutoRunMouse, this);
}

Game::~Game() {
  auto_running_mouse = false; // Signal the auto-run thread to stop
  if (auto_run_mouse.joinable()) {
    auto_run_mouse.join(); // Join the thread before destruction
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, mouse, food);
    if (!snake.alive) {
      // Auto quit game when snake die
      running = false;
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  std::cout << "PlaceFood: " << std::endl;
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!mouse.MouseCell(x, y) && !snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      std::cout << "NewFood: " << x << " " << y << std::endl;
      {
        int new_x = static_cast<int>(mouse.positionX);
        int new_y = static_cast<int>(mouse.positionY);

        SDL_Point start{new_x, new_y};
        std::cout << "Mouse A* search: " << std::endl;
        mouseASearchPath = AStarMouse(start, food);
      }
      return;
    }
  }
}

void Game::NewMouse() {

  mouse.UpdatePosition(random_w(engine), random_h(engine));
  mouse.alive = true;
  std::cout << "New Mouse: " << mouse.positionX << " " << mouse.positionY
            << std::endl;
}

void Game::Update() {
  std::lock_guard<std::mutex> lock(mouse_mutex); // Lock mutex for thread
  // safety
  if (!snake.alive)
    return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  if (new_x == 0 || new_x == static_cast<int>(kGridWidth - 1) || new_y == 0 ||
      new_y == static_cast<int>(kGridHeight - 1)) {
    snake.alive = false;
  }
  // Check if snake eat mouse
  if (mouse.alive && mouse.positionX == new_x && mouse.positionY == new_y) {
    std::cout << "Snake eat Mouse" << std::endl;
    score += 10;
    snake.GrowBody();
    mouse.alive = false;
  }

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    std::cout << "Snake eat Food" << std::endl;
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::AutoRunMouse() {
  while (auto_running_mouse) {

    std::this_thread::sleep_for(
        std::chrono::milliseconds(500)); // mouse delay movement
    std::lock_guard<std::mutex> lock(
        mouse_mutex); // Lock mutex for thread safety
    if (!mouse.alive) {
      mouse.NewMouse();
      {
        int new_x = static_cast<int>(mouse.positionX);
        int new_y = static_cast<int>(mouse.positionY);

        SDL_Point start{new_x, new_y};
        std::cout << "Mouse A* search: " << std::endl;
        mouseASearchPath = AStarMouse(start, food);
      }
    }
    // Calculate the path to the food using A* and move the mouse
    if (!mouseASearchPath.empty()) {
      mouse.positionX = mouseASearchPath[0].x;
      mouse.positionY = mouseASearchPath[0].y;
      std::cout << "Mouse go to position: " << mouseASearchPath[0].x << " "
                << mouseASearchPath[0].y << " Food position: " << food.x << " "
                << food.y << std::endl;
      mouseASearchPath.erase(mouseASearchPath.begin());

      // check mouse eat food
      if (food.x == static_cast<int>(mouse.positionX) &&
          food.y == static_cast<int>(mouse.positionY)) {
        std::cout << "Mouse eat food at: " << mouse.positionX << " "
                  << mouse.positionY << std::endl;
        PlaceFood();
        score -= 10;
      }
    }
  }
}

int Game::heuristic(const SDL_Point &a, const SDL_Point &b) {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<SDL_Point> Game::AStarMouse(const SDL_Point &start,
                                        const SDL_Point &goal) {

  std::cout << "AStarMouse: " << std::endl;
  std::priority_queue<Node> openList;
  std::map<SDL_Point, SDL_Point, bool (*)(const SDL_Point &, const SDL_Point &)>
      cameFrom([](const SDL_Point &a, const SDL_Point &b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
      });
  std::map<SDL_Point, int, bool (*)(const SDL_Point &, const SDL_Point &)>
      gScore([](const SDL_Point &a, const SDL_Point &b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
      });
  std::map<SDL_Point, int, bool (*)(const SDL_Point &, const SDL_Point &)>
      fScore([](const SDL_Point &a, const SDL_Point &b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
      });

  openList.push({start.x, start.y, 0, 0, heuristic(start, goal)});
  gScore[start] = 0;
  fScore[start] = heuristic(start, goal);

  std::vector<SDL_Point> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

  while (!openList.empty()) {
    Node current = openList.top();
    openList.pop();

    SDL_Point currentPos = {current.x, current.y};
    if (currentPos.x == goal.x && currentPos.y == goal.y) {
      std::vector<SDL_Point> path;
      while (cameFrom.find(currentPos) != cameFrom.end()) {
        mouseASearchPath.push_back(currentPos);
        currentPos = cameFrom[currentPos];
      }
      std::reverse(mouseASearchPath.begin(), mouseASearchPath.end());
      return mouseASearchPath;
    }

    for (const auto &dir : directions) {
      SDL_Point neighbor = {current.x + dir.x, current.y + dir.y};

      int tentative_gScore = gScore[currentPos] + 1;
      if (gScore.find(neighbor) == gScore.end() ||
          tentative_gScore < gScore[neighbor]) {
        cameFrom[neighbor] = currentPos;
        gScore[neighbor] = tentative_gScore;
        fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, goal);
        openList.push({neighbor.x, neighbor.y, fScore[neighbor],
                       gScore[neighbor], heuristic(neighbor, goal)});
      }
    }
  }

  return {};
}